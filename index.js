const { readFileSync } = require("fs");

const reference = readFileSync("./src/tests/1/ref.seq").toString();
const sequence = readFileSync("./src/tests/1/seq.txt").toString();

sequence.split("\n").forEach((line) => {
  const index = reference.indexOf(line);
  if (index !== -1 && line) {
    console.log(`${line} found at index ${index}`);
  } else {
    console.log('Not found')
  }
});
