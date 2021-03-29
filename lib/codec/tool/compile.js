

const fs = require('fs');
const path = require('path');
const filepath = path.join(__dirname, '../CMakeLists.txt');
const confpath = path.join(__dirname, '../src/config.h');
const cmds = fs
  .readFileSync(filepath)
  .toString('UTF8')
  .split(/\r?\n/g);

['wasm', 'asm'].forEach(v => {
  let index = cmds.indexOf(`#<-----${v}----->`);
  if (index > -1) {
    if (cmds[index + 1].indexOf('#') != 0) {
      cmds[index + 1] = `#${cmds[index + 1]}`;
    }
  }
});

let type = process.argv[2];
let isBaseline = process.argv[3] == 'baseline';
let isH265 = process.argv[3] == 'h265';
fs.writeFileSync(confpath, `
#ifndef CODEC_CONFIG_H
#define CODEC_CONFIG_H

${isH265 ? '' : '//'}#define USE_OPEN_H265
${isBaseline ? '//' : ''}#define USE_OPEN_H264

#endif //CODEC_CONFIG_H
`);

index = cmds.indexOf(`#<-----${type}----->`);
if (index > -1) {
  cmds[index + 1] = cmds[index + 1].replace('#', '');
}

fs.writeFileSync(filepath, cmds.join('\n'));
