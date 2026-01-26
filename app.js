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
            result.textContent = "";
            result.style.backgroundColor = "white";
            inputDS.value = decodedText;
            stopScanner();
            
        },
        (errorMessage) => {
            // ignore scan errors
        }
    );
});

const inputChiThi = document.getElementById("barcodeChiThi");
const readerDiv2 = document.getElementById("reader2");

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

const inputCaseMark = document.getElementById("barcodeCasemark");
const readerDiv3 = document.getElementById("reader3");
inputCaseMark.addEventListener("click", () => {
    readerDiv3.style.display = "block";
    html5QrCode = new Html5Qrcode("reader");
    html5QrCode.start(
        { facingMode: "environment" }, // camera sau
        {
            fps: 10,
            qrbox: 200
        },
        (decodedText) => {
            inputCaseMark.value = decodedText;
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
    //xacNhan();
    var isErr = false;
    var arrBarCodeChiThi = inputChiThi.value.split("-");
   
    if (arrBarCodeChiThi.length > 1) {

    }else {
        isErr = true;
        alert("Hãy scan barcode tờ Chỉ thị");
        inputChiThi.focus();
    }

    if (!isErr) {
            const ds = inputDS.value;
        if (ds.includes(arrBarCodeChiThi[0]) == true && arrBarCodeChiThi[0].length >= 10) {
            var arrDS = ds.split(arrBarCodeChiThi[0] + " ")
            if (arrDS.length > 1) {
                var rev = arrDS[1].substring(0, 2);
                if (inputCaseMark.value != "") {
                    var arrCasemark = inputCaseMark.value.split("&&");
                    if (arrCasemark.length > 1) {
                        if(arrBarCodeChiThi[0] == arrCasemark[0] && rev == arrCasemark[1]){
                            result.textContent = "OK";
                            result.style.backgroundColor = "rgb(0, 184, 148)";
                        }else{
                            result.textContent = "NG";
                            result.style.backgroundColor = "red";
                        }
                    }else{
                        result.textContent = "NG";
                        result.style.backgroundColor = "red";
                    }
                    
                }else{
                    result.textContent = "DS đã khớp chỉ thị";
                    result.style.backgroundColor = "rgb(243, 156, 18)";
                }
                
            }else{
                result.textContent = "NG";
                result.style.backgroundColor = "red";
            }
        }else{
            result.textContent = "NG";
            result.style.backgroundColor = "red";
        }
        inputDS.value = "";
        inputChiThi.value = "";
        inputCaseMark.value = "";
        inputDS.focus();

    }
});

function xacNhan() {
  
}