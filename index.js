const { readFileSync } = require("fs");

const reference = readFileSync("./src/tests/2/ref.seq").toString();
const sequence = readFileSync("./src/tests/2/seq.txt").toString();

let included = 0;
let lines = 0;
const intervals = [];

const coverage = (ranges) => {
  if (ranges.length < 2) return ranges;

  ranges.sort((a, b) => a[0] - b[0]);

  const mergedRanges = [];
  let previous = ranges[0];

  for (let i = 1; i < ranges.length; i += 1) {
    if (previous[1] >= ranges[i][0]) {
      previous = [previous[0], Math.max(previous[1], ranges[i][1])];
    } else {
      mergedRanges.push(previous);
      previous = ranges[i];
    }
  }

  mergedRanges.push(previous);

  return mergedRanges.reduce((acc, [start, end]) => acc + (end - start), 0);
};

sequence.split("\n").forEach((line, index) => {
  const trimmedLine = line.trim();
  const position = reference.indexOf(trimmedLine);

  if (position !== -1 && line) {
    console.log(`Line ${index} found at index ${position}`);
    included += trimmedLine.length;
    lines++;
    intervals.push([position, position + trimmedLine.length]);
  }
});

const calculatedCoverage = coverage(intervals);

console.log(reference.length, sequence.length);

console.log(
  `${lines} lines ${calculatedCoverage} characters covering ${(
    (calculatedCoverage / reference.length) *
    100
  ).toFixed(2)}% of the genoma reference`
);
