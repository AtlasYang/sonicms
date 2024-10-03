export function unixTimestampToDateString(timestamp) {
  return new Date(timestamp * 1000).toISOString().split("T")[0];
}

export function unixTimestampToDateTimeString(timestamp) {
  return new Date(timestamp * 1000)
    .toISOString()
    .split("T")
    .join(" ")
    .substring(0, 19);
}
