export function unixTimestampToDateString(timestamp) {
  if (timestamp > 9999999999) {
    return new Date(timestamp).toISOString().split("T")[0];
  }
  return new Date(timestamp * 1000).toISOString().split("T")[0];
}

export function unixTimestampToDateTimeString(timestamp) {
  if (timestamp > 9999999999) {
    return new Date(timestamp)
      .toISOString()
      .split("T")
      .join(" ")
      .substring(0, 19);
  }
  return new Date(timestamp * 1000)
    .toISOString()
    .split("T")
    .join(" ")
    .substring(0, 19);
}
