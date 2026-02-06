const inputDS = document.getElementById("barcodeDS");

const readerDiv = document.getElementById("reader");
let html5QrCode;

inputDS.addEventListener("click", () => {

    result.textContent = "";
    result.style.backgroundColor = "white";

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
            inputChiThi.focus();
            
        },
        (errorMessage) => {
            // ignore scan errors
        }
    );
});

const inputChiThi = document.getElementById("barcodeChiThi");
const readerDiv2 = document.getElementById("reader2");

inputChiThi.addEventListener("focus", () => {
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
            xacNhan();
            inputCaseMark.focus();
        },
        (errorMessage) => {
            // ignore scan errors
        }
    );
});

const inputCaseMark = document.getElementById("barcodeCasemark");
const readerDiv3 = document.getElementById("reader3");
inputCaseMark.addEventListener("focus", () => {
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
            xacNhanAndClear();
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
    xacNhanAndClear();
    
});
function getPartcodeRevPartner(vendorCode) {
    var result = ["", ""];
    
    if (vendorCode == "250001348" || vendorCode == "250000051" || vendorCode == "500002006") { 
        result = getPartcodeRevSunway(vendorCode);
    }
    if (vendorCode == "250002768" || vendorCode == "250000069") {
        result = getPartcodeRevSeiyo(vendorCode);
    }
    if (vendorCode == "250001449") { 
        result = getPartcodeRevZhongzu(vendorCode);
    }
    if (vendorCode == "250003738" || vendorCode == "250001478") { 
        result = getPartcodeRevVanlong(vendorCode);
    }
    if (vendorCode == "250000072") { 
        result = getPartcodeRevTaisei(vendorCode);
    }
    if (vendorCode == "250001383") { 
        result = getPartcodeRevBacviet(vendorCode);
    }
    if (vendorCode == "250000076") { 
        result = getPartcodeRevIritani(vendorCode);
    }
    if (vendorCode == "250003179") { 
        result = getPartcodeRevHudson(vendorCode);
    }
    if (vendorCode == "250004228") { 
        result = getPartcodeRevGaoqi(vendorCode);
    }
    if (vendorCode == "250000068") { 
        result = getPartcodeRevSantomas(vendorCode);
    }
    if (vendorCode == "250000054") { 
        result = getPartcodeRevChiyoda(vendorCode);
    }
    if (vendorCode == "250000060") { 
        result = getPartcodeRevKuroda(vendorCode);
    }
    if (vendorCode == "250003288") { 
        result = getPartcodeRevHTMP(vendorCode);
    }
    if (vendorCode == "250000588") { 
        result = getPartcodeRevTOKYO(vendorCode);
    }

    
    // alert(result[1]);
    // alert(result[0]);
   return result;
}
function getPartcodeRevSunway(barcodeChiThi) {
    //Sunway: 302S260010 01 
    //Advanex: 302RV25540 02/-/-/20250518/46020-20251229-101450/6000/1/002-002/-&10010921&1
    //Kdthk for KDTVN: 30C1445010 05/ADH-120AN AA/32/2535

    var arrCasemark = inputCaseMark.value.split(" ");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevSeiyo(barcodeChiThi) { //Seyo hp & seiyo VN: 302RV08021&&01&&2025-4259&&14&&129
    var arrCasemark = inputCaseMark.value.split("&&");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevZhongzu(barcodeChiThi) { //Zhongzu: 302RV14050/ZhongYu/VietNam/20251203/6/192/Rev/02
    var arrCasemark = inputCaseMark.value.split("/");
    return [arrCasemark[0], arrCasemark[7]];
}
function getPartcodeRevBacviet(barcodeChiThi) { //Bac viet: 302RV04150/V1/260112/12/800/A/03/4
    var arrCasemark = inputCaseMark.value.split("/");
    return [arrCasemark[0], arrCasemark[6]];
}
function getPartcodeRevVanlong(barcodeChiThi) { //Vanlong: 302S004060-03_VL.IJ26.2301.08_140_No.17_24-Jan-26
    var arrCasemark = inputCaseMark.value.split("-");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevTaisei(barcodeChiThi) { //Taisei: *&302S046050-04&1500&TAISEI HANOI&-&1161&250609&302S046050-04&*
    barcodeChiThi = barcodeChiThi.replace("*&", "")
    var arrCasemark = inputCaseMark.value.split("-");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevIritani(barcodeChiThi) { //Iritani: 302RV02070/V1/V016/260112/171/50/A/Rev-03/
    var arrCasemark = inputCaseMark.value.split("/");

    var tmp = arrCasemark[1].split("REV-");
    return [arrCasemark[0], tmp[1]];
}
function getPartcodeRevHudson(barcodeChiThi) { //Hudson: 30C0D1922002    12.00000            2601230003960123A0A
    var casemark = inputCaseMark.value;
    return [casemark.substring(1, 10), casemark.substring(11, 12)];
}
function getPartcodeRevGaoqi(barcodeChiThi) { //Gaoqi: 6K06370661\01\85\ZS20251024001
    var arrCasemark = inputCaseMark.value.split("\\");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevSantomas(barcodeChiThi) { //Santomas: &/30C0D31220/V01/0000/251113/0001/01000/NS/----001//&SVN3/F1KVN00115-01-00/TR21KD0111/N/0
    var cm = inputCaseMark.value;
    cm = cm.replace("&/", "");
    cm = cm.replace("/V", "/");
    var arrCasemark = cm.split("/");
    return [arrCasemark[0], arrCasemark[1]];
}
function getPartcodeRevChiyoda(barcodeChiThi) { //Chiyoda : 30C0D14300,R01,7680,7680,KYO001,130557865800010,SOA0289555601,01
    var cm = inputCaseMark.value;
    var arrCasemark = cm.split(",R");
    return [arrCasemark[0], arrCasemark[1].substring(0, 2)];
}

function getPartcodeRevKuroda(barcodeChiThi) { //Kuroda : 00001&3V2LV31230&20260128&050T-02&VK2&1,2,3,4,5,6,7,8&12000&012-0052-00&PIN DRIVE A (白)(3V2LV31230)&VA26010010940&201708003&012&202&1
    var cm = inputCaseMark.value;
    cm = cm.replace("00001&", "");
    var arrCasemark = cm.split("&");
    var tmp = arrCasemark[1].split("-");
    return [arrCasemark[0], tmp[1].substring(0, 2)];
}
function getPartcodeRevHTMP(barcodeChiThi) { //HTMP: &/302YJ21150///201225/0002/0400//02/2/&;20251219000041085865;1;302YJ21150;;PCS;400.00;201225;;;;;KH0084;;238;TP-CANON-TV;1.00;;BGTD0026;BGKP0029;40;10;0;PCS;PCS
    var cm = inputCaseMark.value;
    cm = cm.replace("&/", "");
    var arrCasemark = cm.split("///");
    var tmp = arrCasemark[1].split("//");
    return [arrCasemark[0], tmp[1].substring(0, 2)];
}

function getPartcodeRevTOKYO(barcodeChiThi) { //TOKYO: 303LL24300@PIN READING GUIDE VER.01@MFG251201152@130557155800010@500
//MFG251201152@303LL24300@PIN READING GUIDE VER.01@500@0
    var cm = inputCaseMark.value;
    var arrCasemark = cm.split("@");
    var tmp = arrCasemark[1].split("VER.");
    return [arrCasemark[0], tmp[1].substring(0, 2)];
}
function xacNhanAndClear() {
    xacNhan();
    inputDS.value = "";
    inputChiThi.value = "";
    inputCaseMark.value = "";
    inputDS.focus();
}
function xacNhan() {
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
                    var arrCasemark = getPartcodeRevPartner(arrBarCodeChiThi[2]);
                    
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
    }
}