
var supportSharedBuffer = false;
try {
  supportSharedBuffer = !!new SharedArrayBuffer(0);
} catch (e) {
  // nothing to do...
}

var isWorker = typeof importScripts == "function";
var bridgeName = "__CODE_BRIDGE__" + +new Date();
(isWorker ? self : window)[bridgeName] = {
  onHeader: function(header) {
    Module.postMessage({ type: "header", data: header });
  },
  onMediaInfo: function(mediaInfo) {
    Module.postMessage({ type: "mediaInfo", data: mediaInfo });
  },
  onAudioDataSize: function(data) {
    Module.audioBufferSize = data.size;
    Module.audioBuffer = Module._malloc(Module.audioBufferSize);
    Module._codecSetAudioBuffer(Module.audioBuffer);
  },
  onAudioData: function(data) {
    var timestamp = data.timestamp;
    Module.audioTimestamps.push(timestamp);

    var u8s = Module.HEAPU8.subarray(
      Module.audioBuffer,
      Module.audioBuffer + Module.audioBufferSize
    );

    var output = null;
    if (supportSharedBuffer) {
      output = new Uint8Array(new SharedArrayBuffer(u8s.byteLength));
      output.set(u8s);
    } else {
      output = new Uint8Array(u8s);
    }

    Module._free(Module.audioBuffer);
    Module.audioBuffer = null;
    Module.postMessage(
      {
        type: "audio",
        data: {
          buffer: output.buffer,
          timestamp: timestamp
        }
      },
      supportSharedBuffer ? undefined : [output.buffer]
    );
  },
  onVideoDataSize: function(data) {
    if (Module.videoBuffer == null) {
      Module.videoBufferSize = data.size;
      Module.videoBuffer = Module._malloc(Module.videoBufferSize);
      if (supportSharedBuffer) {
        Module.videoSharedBuffer = new SharedArrayBuffer(data.size);
      }
    }
    Module._codecSetVideoBuffer(Module.videoBuffer);
  },
  onVideoData: function(data) {
    var timestamp = data.timestamp;
    Module.videoTimestamps.push(timestamp);

    var u8s = Module.HEAPU8.subarray(
      Module.videoBuffer,
      Module.videoBuffer + Module.videoBufferSize
    );

    var output = null;
    if (supportSharedBuffer) {
      output = new Uint8Array(Module.videoSharedBuffer);
      output.set(u8s);
    } else {
      output = new Uint8Array(u8s);
    }
    
    Module.postMessage(
      {
        type: "video",
        data: {
          buffer: output.buffer,
          timestamp: timestamp,
          width: data.width,
          height: data.height,
          stride0: data.stride0,
          stride1: data.stride1
        }
      },
      supportSharedBuffer ? undefined : [output.buffer]
    );
  },
  onComplete: function() {
    Module.postMessage({ type: "complete" });
  }
};

var T = {
  audioTimestamps: [],
  videoTimestamps: [],
  audioBufferSize: 0,
  videoBufferSize: 0,
  audioBuffer: null,
  videoBuffer: null,
  postMessage: isWorker ? postMessage.bind(self) : function() {},
  onRuntimeInitialized: function() {
    Module._codecInit();
    var callbackStr = bridgeName.split("");
    callbackStr = callbackStr
      .map(function(v) {
        return v.charCodeAt(0);
      })
      .concat(0);

    var callbackStrData = Module._malloc(callbackStr.length - 1);
    Module.HEAPU8.set(callbackStr, callbackStrData);
    Module._codecSetBridgeName(callbackStrData);

    Module.postMessage({ type: "ready" });
  }
};

Module = Module || {};

for (var key in T) {
  if (T.hasOwnProperty(key)) {
    Module[key] = T[key];
  }
}

Module.onmessage = function(msg) {
  var data = msg.data;
  switch (data.type) {
    case "decode": {
      var buffer = new Uint8Array(data.buffer);
      var data = Module._malloc(buffer.length);
      Module.HEAPU8.set(buffer, data);

      var now = +new Date();
      Module.audioTimestamps = [];
      Module.videoTimestamps = [];
      Module._codecDecode(data, buffer.length);

      var ats = Module.audioTimestamps;
      var vts = Module.videoTimestamps;
      Module.postMessage({
        type: "decode",
        data: {
          consume: +new Date() - now,
          duration: Math.max(
            ats.length > 0 ? ats[ats.length - 1] - ats[0] : 0,
            vts.length > 0 ? vts[vts.length - 1] - vts[0] : 0
          )
        }
      });

      Module._free(data);
      break;
    }
    case "destroy": {
      if (Module.audioBuffer) {
        Module._free(Module.audioBuffer);
      }
      if (Module.videoBuffer) {
        Module._free(Module.videoBuffer);
      }
      Module._codecFree();
      Module.postMessage({ type: "destroy" });
      break;
    }
  }
};

if (isWorker) {
  self.onmessage = Module.onmessage;
}
