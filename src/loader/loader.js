

import StreamLoader from './stream';
import ChunkLoader from './chunk';
import Util from '../util/util';

function Loader({ type = 'chunk', opt }) {
  return type == 'chunk'
    ? new ChunkLoader(opt)
    : new (Util.workerify(StreamLoader, ['read', 'cancel', 'hasData']))(opt);
}

export default Loader;
