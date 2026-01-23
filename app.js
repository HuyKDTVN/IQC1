const inputDS = document.getElementById("barcodeDS");
const readerDiv = document.getElementById("reader");
let html5QrCode;

inputDS.addEventListener("click", () => {
    readerDiv.style.display = "block";

    html5QrCode = new Html5Qrcode("reader");

    html5QrCode.start(
        { facingMode: "environment" }, // camera sau
        {
            fps: 10,
            qrbox: 250
        },
        (decodedText) => {
            inputDS.value = decodedText;
            stopScanner();
            alert("1212")
        },
        (errorMessage) => {
            // ignore scan errors
        }
    );
});

const inputChiThi = document.getElementById("barcodeChiThi");
const readerDiv2 = document.getElementById("reader2");
//let html5QrCode;

inputChiThi.addEventListener("click", () => {
    readerDiv2.style.display = "block";
    html5QrCode = new Html5Qrcode("reader");
    html5QrCode.start(
        { facingMode: "environment" }, // camera sau
        {
            fps: 10,
            qrbox: 250
        },
        (decodedText) => {
            inputChiThi.value = decodedText;
            stopScanner();
        },
        (errorMessage) => {
            // ignore scan errors
        }
    );
});
function stopScanner() {
    if (html5QrCode) {
        html5QrCode.stop().then(() => {
            readerDiv2.style.display = "none";
        });
    }
}
const btnScan = document.getElementById("buttonConfirm");
const result = document.getElementById("ket-qua");
btnScan.addEventListener("click", () => {
    xacNhan();
    var isErr = false;
    var arrBarCodeChiThi = inputChiThi.value.split("-");
    var arrBarCodeDS = inputDS.value.split("220000");
    if (arrBarCodeDS.length > 1) {
        var dsRutGon = arrBarCodeDS[1]
        var arrDsRutGon = dsRutGon.split(" ")
        if (arrDsRutGon.length > 1) {

        }else {
            isErr = true;
            alert("Hãy scan barcode tờ DS");
            inputDS.focus();
        }
    }else {
        isErr = true;
        alert("Hãy scan barcode tờ DS");
        inputDS.focus();
    }
    if (arrBarCodeChiThi.length > 1) {

    }else {
        isErr = true;
        alert("Hãy scan barcode tờ Chỉ thị");
        inputChiThi.focus();
    }
    if (!isErr) {
        if (arrBarCodeChiThi[0] == arrDsRutGon[0] && arrBarCodeChiThi[1] == arrDsRutGon[1]) {
            result.textContent = "OK";
            result.style.backgroundColor = "rgb(0, 184, 148)";
        }else{
            result.textContent = "NG";
            result.style.backgroundColor = "red";
        }
        inputDS.value = "";
        inputChiThi.value = "";
        inputDS.focus();
    }
});

function xacNhan() {
  alert(inputDS.value) 
}