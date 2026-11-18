const choose = document.querySelector("#choose-data");
const input = document.querySelector("#data-folder");
const check = document.querySelector("#check-engine");
const message = document.querySelector("#message");
choose.addEventListener("click", () => input.click());
input.addEventListener("change", () => {
  const files = [...(input.files || [])];
  if (!files.length) return;
  const names = files.map(file => file.webkitRelativePath || file.name);
  const hints = names.filter(name => /\.drs$|empires\.dat$|language\.dll$/i.test(name));
  message.textContent = hints.length
    ? `Đã chọn ${files.length} tệp; phát hiện ${hints.length} tệp có tên phù hợp. Bộ kiểm tra định dạng và nạp dữ liệu sẽ được bổ sung ở giai đoạn tiếp theo. Tệp chưa được tải lên máy chủ.`
    : `Đã chọn ${files.length} tệp. Chưa nhận diện được cấu trúc dữ liệu AoE; hãy chọn thư mục cài đặt gốc. Tệp chưa được tải lên máy chủ.`;
});
check.addEventListener("click", async () => {
  const supported = typeof WebAssembly === "object";
  const webgl = document.createElement("canvas").getContext("webgl2");
  message.textContent = `WebAssembly: ${supported ? "được hỗ trợ" : "không hỗ trợ"} · WebGL 2: ${webgl ? "được hỗ trợ" : "không hỗ trợ"}. Engine game chưa được biên dịch thành wasm trong bản này.`;
});