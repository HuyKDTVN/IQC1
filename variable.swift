//
//  variable.swift
//  LearnChinese
//
//  Created by Huy Le on 25/12/25.
//
import Foundation
import UIKit
import SwiftUI
import UserNotifications

class Variable {
    let production:Bool = false
    var proVersion:Bool = false
    let soTuTrong1Bai: Int = 15 //15
    
    var link_app:String = "https://itunes.apple.com/app/period-calendar-bmi-tracker/1623035516"
    
    var link_dev:String = "https://itunes.apple.com/developer/huy-le/id1279607371"
    var link_privacy: URL = URL(string: "https://sites.google.com/view/fitness-lady-privacy/")!
    var link_terms: URL = URL(string: "https://sites.google.com/view/learn-chinese-terms-of-service/")!
    var link_facebook_message: String = "https://docs.google.com/forms/d/e/1FAIpQLScrl0QySfxqtB5AHlWc1S7dnJf_mcEpkw4VvZ5O_ss-cOI5nw/viewform?usp=publish-editor"
    
    let arrButtonOnboarding = ["Bắt đầu", "Cho phép", "Bắt đầu", "OK", ""]
    
    let userDefaultGroupName: String = "group.com.huy.learn-chinese"
//    func requestNotificationPermission() {
//        UNUserNotificationCenter.current()
//            .requestAuthorization(options: [.alert, .sound, .badge]) { granted, error in
//                if granted {
//                    print("Notification allowed")
//                }
//            }
//    }
    
    let arrNotiDatDuocSteak1 = ["Tiến gần hơn tới mục tiêu. Duy trì chuỗi 2 ngày nào! ✨", "Đừng bỏ lỡ đà tiến bộ! Vào học ngay để giữ chuỗi 2 ngày. ⚡", "Đừng để tiếng Trung dậm chân tại chỗ. Vào giữ chuỗi ngày thứ 2 nào! 🚀", "Vào học ngay! Đừng để công sức ngày đầu tiên tan biến. Duy trì ngày 2!"]
    let arrNotiDatDuocSteak2 = ["Chỉ những người thực sự nghiêm túc với việc học tiếng Trung mới chạm tới ngày thứ 3. Bạn đã sẵn sàng để vượt qua chưa?", "Mỗi ngày 5 phút đang làm bạn thay đổi. Đừng để đứt quãng, vào tiếp nối ngày thứ 3 nào!"]
    let arrNotiDatDuocSteak3 = ["Bạn đã nỗ lực suốt 3 ngày qua. Đừng để chuỗi Streak quay về số 0 vào lúc này. Vào học thôi!"]
    let arrNotiDatDuocSteak4 = ["Rất ít người kiên trì được đến ngày thứ 5. Hãy giữ vững phong độ và nối dài chuỗi Streak ngay nào!", "5 ngày liên tiếp là một con số ấn tượng. Đừng dừng lại khi tiếng Trung đang dần trở nên dễ dàng hơn!", "Tiếng Trung không khó khi bạn có sự bền bỉ của ngày hôm nay. Tiếp tục hành trình ngày thứ 5 thôi!"]
    let arrNotiDatDuocSteak4Title = ["Bạn thuộc nhóm 20% xuất sắc! ✨", "Nhìn lại xem, bạn đã đi xa thế nào!", "Kiên trì là chìa khóa ❤️"]
    
    let arrNotiDatDuocSteak5 = ["Bạn đã nỗ lực rất nhiều để có chuỗi 5 ngày. Đừng để nó biến mất chỉ vì một phút lười nhé. Vào học ngay!"]
    let arrNotiDatDuocSteak6 = ["Chỉ còn một bước nữa thôi để tròn chuỗi 7 ngày rực rỡ. Đừng dừng lại khi vinh quang ngay trước mắt!"]
    
    let arrNotiDatDuocSteak7 = ["Vốn từ của bạn đang tăng lên mỗi ngày. Vào học ngay để nối dài chuỗi ngày thứ [chuoingay] và làm giàu kho từ vựng nào!",
                                "Bạn đã học được [sotu] từ! Duy trì chuỗi ngày thứ [chuoingay] để không lãng phí đà tiến bộ bạn nhé.",
                                "[sotu] từ vựng đã nằm trong tay bạn. Vào học ngay để giữ chuỗi ngày thứ [chuoingay] và tăng tốc vốn từ nào!",
                                "Bạn đã học [sotu] từ. Đừng dừng lại! Duy trì chuỗi ngày thứ [chuoingay] để bứt phá tiếng Trung ngay.",
                                "Đã có [sotu] từ vựng trong túi, đừng để vốn từ đứng yên. Vào điểm danh giữ chuỗi ngày thứ [chuoingay] thôi!",
                                "Với [sotu] từ, bạn đang tiến rất xa. Tiếp tục chuỗi ngày thứ [chuoingay] để vốn từ vựng thêm phong phú nhé!"
                            ]
    let arrNotiDatDuocSteak7Title = ["Thêm 5 từ, giữ 1 chuỗi!",
                                     "Đừng để vốn từ \"đứng yên\" 💡",
                                     "Tiếp đà thăng tiến! 🚀",
                                     "Giữ đà tiến bộ! ✨",
                                     "Đừng để công sức lãng phí!",
                                     "Sắp thành \"Bậc thầy\" rồi! 👑"]
    
    func updateCapDoBaiHocCaoNhat(capDo: Int, baiHoc: Int) -> Bool {
        let defaults = UserDefaults(suiteName: Variable().userDefaultGroupName)
        var hasUpdate = false
        let capDoHienTai = getUserDefaultInt(name: "capDoCaoNhat")
        let baiHocHienTai = getUserDefaultInt(name: "baiHocCaoNhat")
        if capDo > capDoHienTai {
            hasUpdate = true
            defaults?.set(capDo, forKey: "capDoCaoNhat")
        }
        if baiHoc > baiHocHienTai {
            hasUpdate = true
            defaults?.set(capDo, forKey: "baiHocCaoNhat")
        }
        
        return hasUpdate
    }
    func requestNotificationPermission() async -> Bool {
        do {
            return try await UNUserNotificationCenter.current()
                .requestAuthorization(options: [.alert, .sound, .badge])
        } catch {
            return false
        }
    }
    
    
    func getUserDefaultBool(name: String) -> Bool {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let result = defaults?.bool(forKey: name) ?? false
        return result
    }
    
    func getUserDefaultInt(name: String) -> Int {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let result = defaults?.integer(forKey: name) ?? 0
        return result
    }
    func getUserDefaultString(name: String) -> String {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let result = defaults?.string(forKey: name) ?? ""
        return result
    }
    func getSoTuDaHoc() -> Int {
            return 100
    }
    func getUserDefaultArrInt(name: String) -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let arr = defaults?.array(forKey: name) as? [[Int]] ?? []
        return arr
    }
    func getUserDefaultArrInt1(name: String) -> [Int] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let arr = defaults?.array(forKey: name) as? [Int] ?? []
        return arr
    }
    
    
    func getTenCapDo(capDo: Int) -> String {
        var result = "HSK \(capDo)"
        
        if capDo == 8 {
            result = "50 bộ thủ thông dụng"
        }
        if capDo == 7 {
            result = "214 bộ thủ"
        }
        if capDo == 9 {
            result = "Từ khó"
        }
        return result
    }
    func getSoBai(soBai: Int) -> String {
        var result = "Bài \(soBai)"
        if soBai == 99 {
            result = "Ôn tập tất cả"
        }
        return result
    }
    func getDataHocGanNhat() -> [Vocabulary] {
        var result: [Vocabulary] = []
        let variable:Variable = Variable()
        let capDo = variable.getUserDefaultInt(name: "capDoCaoNhat")
        let arrCauHoi: [Int] = variable.getUserDefaultArrInt1(name: "arrCauHoiGanNhat")
        
        var hsk: [Vocabulary] = []
        
        if capDo != 9 {
            switch capDo {
            case 1:
                hsk = variable.loadVocabulary(hsk: "hsk1")
            case 2:
                hsk = variable.loadVocabulary(hsk: "hsk2")
            case 3:
                hsk = variable.loadVocabulary(hsk: "hsk3")
           
            case 8:
                hsk = variable.loadVocabulary(hsk: "bothu50")
            case 7:
                hsk = variable.loadVocabulary(hsk: "bothuall")
            default:
                hsk = variable.loadVocabulary(hsk: "hsk1")
            }
            if arrCauHoi.count > 0 {
                for i in arrCauHoi {
                    result.append(hsk[i])
                }
            }
        }
        
        return result
    }
    
    func setDefaultForData() {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        defaults?.set(true, forKey: "amThanh")
        defaults?.set(true, forKey: "giongDoc")
        defaults?.set(true, forKey: "danhNgon")
        defaults?.set(true, forKey: "nhacnhotumoi")
        
    }
    func get4DapAn(voca: [Vocabulary], viTriTu: Int) -> [String] {
        let random = Int.random(in: 1...4)
//        print("test tu kho")
//        print(voca)
        var arrResult: [String] = ["\(random)", "", "", "", "", ""]
        arrResult[random] = voca[viTriTu].vietnamese
        if voca.count < 8 { // su dung hsk1 neu khong du 4 dap an
            let vocaHSK1 = loadVocabulary(hsk: "hsk1")
            for i in 1...4 where i != random {
                if viTriTu > 75 {
                    arrResult[i] = vocaHSK1[viTriTu - i].vietnamese
                } else {
                    arrResult[i] = vocaHSK1[viTriTu + i].vietnamese
                }
            }
        } else{
            for i in 1...4 where i != random {
                if viTriTu > Int(voca.count / 2)    {
                    arrResult[i] = voca[viTriTu - i].vietnamese
                } else {
                    arrResult[i] = voca[viTriTu + i].vietnamese
                }
            }
        }
        
        arrResult[5] = voca[viTriTu].chinese
        return arrResult
    }
    func getArrBaiHoc(a: Int, doDaiArray: Int) -> [Int] {
        var start = (a - 1) * 15
        var end = start + 14
        if end > doDaiArray - 1 {
            end = doDaiArray - 1
        }
        if a == 99 {
            start = 0
            end = doDaiArray - 1
        }
        
        return Array(start...end)
    }
    func getPercentFinish(capDo: Int) -> CGFloat {
        var result: CGFloat = 0
        
        var diemTong:CGFloat = 0
        var diemHienTai:Int = 0
        
        let hsk = loadVocabulary(hsk: "hsk\(capDo)")
        diemTong = ceil(CGFloat(hsk.count) / 15) * 15
        
        let dataHSK: [[Int]]
        switch capDo {
            case 1:
                dataHSK = getDataHSK1()
            case 2:
                dataHSK = getDataHSK2()
            case 3:
                dataHSK = getDataHSK3()
            default:
                dataHSK = getDataHSK1()
            }
        
        for r in dataHSK {
            for val in r {
                diemHienTai = diemHienTai + val
            }
        }
        result = CGFloat(diemHienTai) / diemTong
        return result
    }
    func capDoHienTai() -> Int {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var result: Int = defaults?.integer(forKey: "cap_do_hien_tai") ?? 0
        if result == 0 {
            result = 1
        }
        return result
    }
    func checkToday(thu: Int) -> Bool {
        var result = false
        
        var dayOfWeek:Int = Date().dayNumberOfWeek()!
        if dayOfWeek == 1 {
            dayOfWeek = 8
        }
        if thu + 1 == dayOfWeek {
            result = true
        }
        return result
        
    }
    func getArrChuoiNgayInWeek() -> [Int] {
        var result: [Int] = [0, 0, 0, 0, 0, 0, 0]
        let soNgay = getChuoiNgay()
        
        var dayOfWeek:Int = Date().dayNumberOfWeek()!
        if dayOfWeek == 1 {
            dayOfWeek = 8
        }
        
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        let dayOfYearInDataBase = defaults?.integer(forKey: "dayofyear")
        let dayOfYear = getDayOfYear()
        
        if dayOfYear == dayOfYearInDataBase {
            for i in 0..<soNgay{
                if dayOfWeek - 2 - i >= 0 {
                    result[dayOfWeek - 2 - i] = 1
                }
                else {
                    break;
                }
            }
        } else{
            for i in 0..<soNgay{
                if dayOfWeek - 2 - i - 1 >= 0 {
                    result[dayOfWeek - 2 - i - 1] = 1
                }
                else {
                    break;
                }
            }
        }
        
        print(result)
        return result
        
    }
    func getChuoiNgay() -> Int {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var result = defaults?.integer(forKey: "songay") ?? 0
        let dayOfYearInDataBase = defaults?.integer(forKey: "dayofyear") ?? 0
        let dayOfYear = getDayOfYear()
        print("ngay mo app so voi ngya hien tai: \(dayOfYearInDataBase) : \(dayOfYear)")
        
        if(dayOfYear - dayOfYearInDataBase >= 2) {
            defaults?.set(0, forKey: "songay")
           result = 0
        }
        
       
        return result
    }
    
    func updateChuoiNgay() -> Bool {//true - co update; false - khong update
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var result = false
        let soNgay = getChuoiNgay()
        let dayOfYearInDataBase = defaults?.integer(forKey: "dayofyear")
        let dayOfYear = getDayOfYear()
        if(dayOfYear != dayOfYearInDataBase) {
            defaults?.set(dayOfYear, forKey: "dayofyear")
            defaults?.set(soNgay + 1, forKey: "songay")
            result = true
        }
        return result
    }
    func hasNewDay() -> Bool {
       var result = false
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
       let dayOfYearInDataBase = defaults?.integer(forKey: "dayofyear")
       let dayOfYear = getDayOfYear()
       if(dayOfYear != dayOfYearInDataBase) {
          result = true
       }
       
       return result
    }
    
    func getDayOfYear() -> Int {
       // Lấy ngày hiện tại
       let currentDate = Date()

       // Tạo một đối tượng Calendar
       let calendar = Calendar.current

       // Lấy thông tin ngày trong năm
       let dayOfYear = calendar.ordinality(of: .day, in: .year, for: currentDate)

       return  dayOfYear!
    }
    func capDoHienTaiPerCent(capDo: Int) -> Int {
        var result = 0
        //let result: Int = UserDefaults.standard.integer(forKey: "cap_do_hien_tai_percent")
        var arr: [[Int]] = [[]]
        
        var soBaiDaHoanThanh = 0
        switch capDo {
        case 1:
            arr = getDataHSK1()
        case 2:
            arr = getDataHSK2()
        case 3:
            arr = getDataHSK3()
        default:
            arr = getDataHSK1()
        }
        for row in arr {
            for value in row {
                soBaiDaHoanThanh = soBaiDaHoanThanh + value
            }
        }
        
        result = soBaiDaHoanThanh * 100 / (15 * 3 * 5) //15 bai lon - moi bai gom 3 bai nho - moi bai nho 5 lan can hoc
        if result > 100 {
            result = 100
        }
        return result
    }
    func getDataDaHocGanDay() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_da_hoc_gan_day") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 2), count: 40)
            defaults?.set(arr, forKey: "data_da_hoc_gan_day")
        }
        return arr
    }
    func getRatioScreen() -> Double {
       let screenSize: CGRect = UIScreen.main.bounds
       //print("ratio: \(screenSize.height / screenSize.width) - \(screenSize.height) - \(screenSize.width)")
       return screenSize.height / screenSize.width
    }
    func getWidthScreen() -> Int {
       let screenSize: CGRect = UIScreen.main.bounds
       return Int(screenSize.width)
    }
    func getDataLiked() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_liked") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 100)
            defaults?.set(arr, forKey: "data_liked")
        }
        return arr
    }
    func getDataVocaLiked() -> [Vocabulary] {
        var result: [Vocabulary] = []
        
        let hsk1 = loadVocabulary(hsk: "hsk1")
        let hsk2 = loadVocabulary(hsk: "hsk2")
        let hsk3 = loadVocabulary(hsk: "hsk3")
        let bothu50 = loadVocabulary(hsk: "bothu50")
        let bothuall = loadVocabulary(hsk: "bothuall")
        
        let dataLiked = getDataLiked()
        for r in dataLiked {
            if r[0] > 0 && r[1] > 0 {
                switch r[0] {
                case 1:
                    result.append(hsk1[r[1] - 1])
                case 2:
                    result.append(hsk2[r[1] - 1])
                case 3:
                    result.append(hsk3[r[1] - 1])
                case 8:
                    result.append(bothu50[r[1] - 1])
                case 7:
                    result.append(bothuall[r[1] - 1])
                default:
                    result.append(hsk1[r[1] - 1])
                }
            }
        }
        return result
    }
    
    func removeDataLiked(capDo: Int, viTriTuCong1: Int) -> Bool {
        //true - co de remove; false - khong tim thay de remove
        
        var flag = false
        var dataLiked = getDataLiked()
        
        if capDo != 9 {
            for (i, r) in dataLiked.enumerated() {
                if capDo == r[0] && viTriTuCong1 == r[1] {
                    dataLiked[i][0] = 0
                    dataLiked[i][1] = 0
                    flag = true
                    break
                }
            }
        } else {
            //dataLiked.remove(at: viTriTuCong1)
            dataLiked[viTriTuCong1][0] = 0
            dataLiked[viTriTuCong1][1] = 0
            flag = true
        }
        if flag {
            let sortedArr = dataLiked.sorted { a, b in
                a != [0, 0] && b == [0, 0]
            }
            let defaults = UserDefaults(suiteName: userDefaultGroupName)
            defaults?.set(sortedArr, forKey: "data_liked")
            print("dataLiked")
            print(sortedArr)
        }
        return flag
    }
    
    func getTotalLinked() -> Int {
        var result = 0
        let dataLiked = getDataLiked()
        for r in dataLiked {
            if r[0] > 0 && r[1] > 0 {
                result = result + 1
            }
        }
        return result
    }
    func hasHanziLiked(capDo: Int, viTriTuCong1: Int, dataLiked: [[Int]]) -> Bool {
        //true - da liked; false - chua liked
        var flag = false
        //let dataLiked = getDataLiked()
        if capDo != 9 {
            for r in dataLiked {
                if capDo == r[0] && viTriTuCong1 == r[1] {
                    print("Da tim thay")
                    print (capDo)
                    print(viTriTuCong1)
                    flag = true
                    break
                }
            }
        }else{
            flag = true
        }
        
        return flag
    }
    func getDayOfW(thu: Int) -> String {
        var result = "T\(thu)"
        if thu == 8 {
            result = "CN"
        }
        return result
    }
    func addDataLiked(capDo: Int, viTriTuCong1: Int) -> Bool {
        // true - du cho update; false - da du 100 phan tu, khong the update them
        
        var viTriInsert = 0
        var flag = false
        var dataLiked = getDataLiked()
        for (i, r) in dataLiked.enumerated() {
            if r[0] == 0 && r[1] == 0 {
                viTriInsert = i
                flag = true
                break
            }
        }
        
        if flag { // du dieu kien update
            dataLiked[viTriInsert][0] = capDo
            dataLiked[viTriInsert][1] = viTriTuCong1
            let defaults = UserDefaults(suiteName: userDefaultGroupName)
            defaults?.set(dataLiked, forKey: "data_liked")
        }
        return flag
    }
    
    func getDataHSK1() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_hsk1") as? [[Int]] ?? []
        //print("mang hsk1")
        //print(arr)
        if arr.count == 0 {
            //print("mang rong")
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 200)
            defaults?.set(arr, forKey: "data_hsk1")
            //print(arr)
        }
        return arr
    }
    func getDataHSK2() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_hsk2") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 200)
            defaults?.set(arr, forKey: "data_hsk2")
        }
        return arr
    }
    func getDataHSK3() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_hsk3") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 400)
            defaults?.set(arr, forKey: "data_hsk3")
        }
        return arr
    }
    func getDataBoThu50() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_bothu50") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 100)
            defaults?.set(arr, forKey: "data_bothu50")
        }
        return arr
    }
    func getDataBoThuAll() -> [[Int]] {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        var arr = defaults?.array(forKey: "data_bothu_all") as? [[Int]] ?? []
        if arr.count == 0 {
            arr = Array(repeating: Array(repeating: 0, count: 3), count: 215)
            defaults?.set(arr, forKey: "data_bothu_all")
        }
        return arr
    }
    func saveDataHSK(arrHsk: [[Int]], capDo: Int) -> Void {
        let defaults = UserDefaults(suiteName: userDefaultGroupName)
        switch capDo {
        case 1:
            defaults?.set(arrHsk, forKey: "data_hsk1")
        case 2:
            defaults?.set(arrHsk, forKey: "data_hsk2")
        case 3:
            defaults?.set(arrHsk, forKey: "data_hsk3")
        case 9:
            defaults?.set(arrHsk, forKey: "data_liked")
        case 8:
            defaults?.set(arrHsk, forKey: "data_bothu50")
        case 7:
            defaults?.set(arrHsk, forKey: "data_bothu_all")
        default:
            defaults?.set(arrHsk, forKey: "data_hsk1")
        }
    }
    func loadVocabulary(hsk : String) -> [Vocabulary] {
        guard let url = Bundle.main.url(forResource: hsk, withExtension: "json"),
              let data = try? Data(contentsOf: url),
              let decoded = try? JSONDecoder().decode(VocabularyResponse.self, from: data)
        else {
            return []
        }
        return decoded.vocabulary
    }
    

    func getDanhNgon() -> String {
//       var selectNgonNgu = "en"
//       let langStr = Locale.current.languageCode
//       if(langStr == "vi"){
//          selectNgonNgu = "vi"
//       }
//       
//       if(langStr == "zh"){
//          selectNgonNgu = "zh"
//       }
//       
//       if(langStr == "de"){
//          selectNgonNgu = "de"
//       }
        let selectNgonNgu = "vi"
       //let selectNgonNgu = UserDefaults.standard.string(forKey: "language") ?? "en"
       return (arrDN[selectNgonNgu]?.randomElement())!
       //return "Bạn không cần là bất cứ ai, hãy là chính mình."
    }
    let loaiBaiHoc = ["Ghi nhớ từ vựng với flashcard", "Bài tập trắc nghiệm", "Luyện viết chữ Hán"]
    let loaiBaiHocIcon = ["flashcard", "trac-nghiem", "luyen-viet"]
   var arrDN = [
       "vi" : [
        "Không có áp lực thì không có kim cương.",
        "Biết thêm một ngoại ngữ là sống thêm một cuộc đời.",
        "Mỗi ngày một chút, ngoại ngữ sẽ theo bạn cả đời.",
        "Ngoại ngữ không khó, khó là thiếu kiên trì.",
        "Kiên trì hôm nay, giỏi ngoại ngữ ngày mai.",
        "Nói sai còn hơn không nói.",
        "Ngoại ngữ không phải năng khiếu, mà là thói quen.",
        "Học ngoại ngữ là hành trình dài, không phải cuộc chạy nước rút.",
        "Ngoại ngữ giỏi bắt đầu từ sự chăm chỉ.",
        "Ngoại ngữ không cần giỏi ngay, chỉ cần học đều.",
        "Không có đường tắt trong việc học ngoại ngữ.",
        "Ngoại ngữ là kỹ năng, không phải phép màu.",
        "Đều đặn quan trọng hơn thông minh.",
        "Không tiến nhanh cũng được, đừng dừng lại.",
        "Mệt thì nghỉ, đừng bỏ.",
        "Ngoại ngữ là kết quả của thói quen tốt.",
        "Đừng sợ sai, chỉ sợ không học.",
        "Bỏ một ngày là mất cơ hội.",
        "Lười hôm nay, khó ngày mai.",
        "Kiên trì là chìa khóa thành công."

       ],
       "en" : [
        "Không có áp lực thì không có kim cương.",
        "Biết thêm một ngoại ngữ là sống thêm một cuộc đời.",
        "Mỗi ngày một chút, ngoại ngữ sẽ theo bạn cả đời.",
        "Ngoại ngữ không khó, khó là thiếu kiên trì.",
        "Kiên trì hôm nay, giỏi ngoại ngữ ngày mai.",
        "Nói sai còn hơn không nói.",
        "Ngoại ngữ không phải năng khiếu, mà là thói quen.",
        "Học ngoại ngữ là hành trình dài, không phải cuộc chạy nước rút.",
        "Ngoại ngữ giỏi bắt đầu từ sự chăm chỉ.",
        "Ngoại ngữ không cần giỏi ngay, chỉ cần học đều.",
        "Không có đường tắt trong việc học ngoại ngữ.",
        "Ngoại ngữ là kỹ năng, không phải phép màu.",
        "Đều đặn quan trọng hơn thông minh.",
        "Không tiến nhanh cũng được, đừng dừng lại.",
        "Mệt thì nghỉ, đừng bỏ.",
        "Ngoại ngữ là kết quả của thói quen tốt.",
        "Đừng sợ sai, chỉ sợ không học.",
        "Bỏ một ngày là mất cơ hội.",
        "Lười hôm nay, khó ngày mai.",
        "Kiên trì là chìa khóa thành công."

       ]
   ]
}
extension Date {
    // returns an integer from 1 - 7, with 1 being Sunday and 7 being Saturday
    func dayNumberOfWeek() -> Int? {
        return Calendar.current.dateComponents([.weekday], from: self).weekday
    }
}
