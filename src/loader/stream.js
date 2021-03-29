
export default function() {
  let supportSharedBuffer = false;
  try {
    supportSharedBuffer = !!new SharedArrayBuffer(0);
  } catch (e) {
    // nothing to do...
  }

  function concat(i, j) {
    const buffer = new Uint8Array(i.length + j.length);
    buffer.set(new Uint8Array(i), 0);
    buffer.set(new Uint8Array(j), i.length);
    return buffer;
  }

  function slice(buffer, startIndex, endIndex) {
    if (!endIndex || endIndex - startIndex > buffer.length) {
      endIndex = buffer.length;
    }

    if(supportSharedBuffer){
      let sharedBuffer = new SharedArrayBuffer(endIndex - startIndex);
      let result = new Uint8Array(sharedBuffer);
      result.set(buffer.subarray(startIndex, endIndex));
      return result;
    }else{
      return buffer.subarray(startIndex, endIndex);
    }
  }

  function StreamLoader({ url, chunkSize = 256 * 1024 }) {
    this.url = url;
    this.done = false;
    this.reader = null;
    this.chunkSize = chunkSize;
    this.data = new Uint8Array(0);
  }

  StreamLoader.prototype.hasData = function() {
    return !this.done || (this.done && this.data.length);
  };

  StreamLoader.prototype.read = function() {
    if (this.data.length < this.chunkSize) {
      if (this.done) {
        return this._getChunk();
      }
      return this._request().then(() => {
        return this._getChunk();
      });
    }
    return this._getChunk();
  };

  StreamLoader.prototype.cancel = function() {
    this.data = null;
    this.reader = null;
    this.done = true;
  };

  StreamLoader.prototype._getChunk = function() {
    return new Promise(resolve => {
      const buffer = slice(this.data, 0, this.chunkSize);
      this.data = slice(
        this.data, 
        this.data.length <= this.chunkSize ? this.data.length : this.chunkSize
      );
      resolve(buffer);
    });
  };

  StreamLoader.prototype._request = function() {
    if (this.reader) {
      return this.reader.read().then(result => {
        let { value, done } = result;
        value = new Uint8Array(value ? value : 0);
        this.data = concat(this.data, value);
        if (done) {
          this.done = true;
        } else if (this.data.length < this.chunkSize) {
          return this._request();
        }
      });
    } else {
      return fetch(this.url, {
        method: 'GET'
      })
        .then(resp => {
          const { status, statusText } = resp;
          if (status < 200 || status > 299) {
            return resp.text().then(text => {
              self.postMessage({
                type: 'event',
                data: {
                  type: 'loadError',
                  data: { status, statusText, detail: text }
                }
              });
            });
          }

          self.postMessage({ type: 'event', data: { type: 'loadSuccess' } });
          this.reader = resp.body.getReader();
          return this._request();
        })
        .catch(e => {
          self.postMessage({
            type: 'event',
            data: {
              type: 'loadError',
              data: { status: -1, statusText: 'unknown error', detail: e }
            }
          });
        });
    }
  };

  let loader = null;
  self.onmessage = message => {
    const { type, id, data } = message.data;
    if (type == 'constructor') {
      loader = new StreamLoader(data);
      self.postMessage({ id });
    } else if (type == 'hasData') {
      self.postMessage({
        id,
        data: !loader ? false : loader.hasData()
      });
    } else if (type == 'read') {
      if (loader) {
        loader.read().then(data => {
          data = new Uint8Array(data);
          self.postMessage({ id, data }, [data.buffer]);
        });
      } else {
        self.postMessage({ id, data: new Uint8Array(0) });
      }
    } else if (type == 'cancel') {
      if (loader) {
        loader.cancel();
      }
      self.postMessage({ id, destroy: true });
    }
  };
}
