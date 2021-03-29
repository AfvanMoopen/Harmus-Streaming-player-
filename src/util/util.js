

import EventEmitter from 'eventemitter3';
import inherits from 'inherits';

export default {
  isWeChat() {
    return /MicroMessenger/i.test(window.navigator.userAgent);
  },
  workerify(func, methods = []) {
    const funcStr = this.getFuncBody(func.toString());
    function __Worker__(data) {
      EventEmitter.call(this);
      this.id = 0;
      this.resolves = [];

      const blob = new Blob([funcStr], { type: 'text/javascript' });
      this.url = URL.createObjectURL(blob);
      this.worker = new Worker(this.url);
      this.worker.onmessage = message => {
        const { id, data, destroy, type } = message.data;
        if (destroy) {
          this.resolves = [];
          URL.revokeObjectURL(this.url);
          this.worker.terminate();
          this.worker = null;
        } else if (type == 'event') {
          this.emit(data.type, data.data);
        } else {
          for (let i = this.resolves.length - 1; i >= 0; i--) {
            if (id == this.resolves[i].id) {
              this.resolves[i].resolve(data);
              this.resolves.splice(i, 1);
              break;
            }
          }
        }
      };

      this.worker.postMessage({ type: 'constructor', id: this.id++, data });
    }

    inherits(__Worker__, EventEmitter);

    for (let i = 0; i < methods.length; i++) {
      const type = methods[i];
      __Worker__.prototype[type] = function(data) {
        return new Promise((resolve, reject) => {
          const id = this.id++;
          this.resolves.push({ id, resolve, reject });
          if (this.worker) {
            this.worker.postMessage({ type, id, data });
          }
        });
      };
    }

    return __Worker__;
  },
  getFuncBody(funcStr) {
    return funcStr
      .trim()
      .match(/^function\s*\w*\s*\([\w\s,]*\)\s*{([\w\W]*?)}$/)[1];
  }
};
