//Select favorite workout
//  Workout.swift
//  sturdy-chest-abs-workout
//
//  Created by huy on 30/11/2017. Modified by huy on 28/9/2024
//  Copyright © 2017 huy. All rights reserved.
//

import UIKit
import SwiftMessages
import SwiftUI
import FirebaseAnalytics

class Workout: UIViewController {
    private var baiTapItems: [UIView] = []
    private var itemViews: [UIView] = []
    private var layoutConstraints: [NSLayoutConstraint] = []
    var variable = Variable()
    let titleThuThach: UILabel = {
       let textView = UILabel()
       textView.textColor = .black
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    let wrapDescThuThach: UIView = {
       let tView = UIView()
       //tView.backgroundColor = .black
       tView.translatesAutoresizingMaskIntoConstraints = false
       return tView
    }()
   
    let descThuThach: UILabel = {
       let textView = UILabel()
       textView.textColor = .white
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    let wrapContentThuThach: UIView = {
       let tView = UIView()
        //tView.backgroundColor = variable.bgHongNhat3
       tView.translatesAutoresizingMaskIntoConstraints = false
       return tView
    }()
    let wrapContentThuThachLeft: UIView = {
       let tView = UIView()
        //tView.backgroundColor = variable.bgHongNhat3
       tView.translatesAutoresizingMaskIntoConstraints = false
       return tView
    }()
    let wrapContentThuThachRight: UIView = {
       let tView = UIView()
        //tView.backgroundColor = variable.bgHongNhat3
       tView.translatesAutoresizingMaskIntoConstraints = false
       return tView
    }()
    
    let contentThuthachTitleLeft: UILabel = {
       let textView = UILabel()
       textView.textColor = .white
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    let contentThuthachTitleRight: UILabel = {
       let textView = UILabel()
       textView.textColor = .white
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    
    let contentThuthachDescLeft: UILabel = {
       let textView = UILabel()
       textView.textColor = .white
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    let contentThuthachDescRight: UILabel = {
       let textView = UILabel()
       textView.textColor = .white
       textView.backgroundColor = UIColor.clear
       textView.translatesAutoresizingMaskIntoConstraints = false
       return textView
    }()
    override func viewWillTransition(
        to size: CGSize,
        with coordinator: UIViewControllerTransitionCoordinator
    ) {
        super.viewWillTransition(to: size, with: coordinator)

        coordinator.animate(alongsideTransition: nil) { [weak self] _ in
            self?.updateLayout()
        }
    }
    
    func updateLayout() {
        NSLayoutConstraint.deactivate(layoutConstraints)
        layoutConstraints.removeAll()

        //print("man hinh hien tai: \(view.bounds.size.width)")
        
        let screensize: CGRect = UIScreen.main.bounds
        let screenWidth = screensize.width
        var withItem: CGFloat = 0.2
        var fixSmallWidthIpad:CGFloat = 1
        if UIDevice.current.userInterfaceIdiom == .pad   {
            withItem = 0.12
            fixSmallWidthIpad = 0.8
            
        }
        if traitCollection.horizontalSizeClass == .regular &&  UIDevice.current.userInterfaceIdiom != .pad{
            withItem = 0.1
            fixSmallWidthIpad = 1.2
        }
        if traitCollection.horizontalSizeClass == .compact &&  UIDevice.current.userInterfaceIdiom != .pad {
            withItem = 0.2
            fixSmallWidthIpad = 1
        }
        
        for i in 0..<baiTapItems.count {
            let baiTap = baiTapItems[i]
            for j in 0..<itemViews.count {
                let itemChild1 = itemViews[j]
                let leftChild1 = CGFloat(j) * (screenWidth - 40 - 10) * 0.2
                
                layoutConstraints += [
                    itemChild1.leadingAnchor.constraint(equalTo: baiTap.leadingAnchor, constant: leftChild1),
                    itemChild1.widthAnchor.constraint(equalTo: baiTap.widthAnchor, multiplier: withItem),
                ]
                
                
                itemChild1.layer.cornerRadius = withItem * fixSmallWidthIpad * (screenWidth - 100) / 2
                let leftChild2 = CGFloat(j) * (screenWidth - 40 - 10) * 0.2 * fixSmallWidthIpad
                
                layoutConstraints += [
                    itemChild1.leadingAnchor.constraint(equalTo: baiTap.leadingAnchor, constant: leftChild2),
                    itemChild1.widthAnchor.constraint(equalTo: baiTap.widthAnchor, multiplier: withItem),
                    //wrapChild.layer.cornerRadius = withItem * (screenWidth - 100) / 2
                ]
            }
            
            
        }
        
        NSLayoutConstraint.activate(layoutConstraints)
    }
   
    func checkConnect() {
        if variable.proVersion == false {
            if Reachability.isConnectedToNetwork() == true {
                print("Internet connection OK")
            } else {
                print("Internet connection FAILED")
                DispatchQueue.main.asyncAfter(deadline: .now() + .seconds(1), execute: {
                    //hide all item
                })
                let warning = MessageView.viewFromNib(layout: .cardView)
                warning.configureTheme(.error)
                warning.configureDropShadow()
                warning.configureContent(title: NSLocalizedString("errNetWork", comment: ""), body:NSLocalizedString("checkNetWork", comment: ""))
                warning.button?.isHidden = true
                var warningConfig = SwiftMessages.defaultConfig
                warningConfig.duration = .forever
                warningConfig.dimMode = .blur(style: .dark, alpha: 1, interactive: true)
                warningConfig.presentationContext = .window(windowLevel: UIWindow.Level.statusBar)
                SwiftMessages.show(config: warningConfig, view: warning)
            }
        }
    }
    private func updateLayoutForState(isUnfolded: Bool) {
        if isUnfolded {
            print("Vừa MỞ máy out -> Chuyển sang 2 cột / Split View")
        } else {
            print("Vừa GẬP máy into -> Chuyển sang 1 cột đơn")
        }
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Đăng ký theo dõi sự thay đổi của Horizontal Size Class
        if #available(iOS 27.0, *) {
            registerForTraitChanges([UITraitHorizontalSizeClass.self]) { (self: Self, previousTraitCollection: UITraitCollection) in
                let isUnfolded = self.traitCollection.horizontalSizeClass == .regular
                self.updateLayoutForState(isUnfolded: isUnfolded)
            }
        }
        
        setBackImage()
        SwiftMessages.hide()
        checkConnect()
        self.title = NSLocalizedString("selectWorkLove", comment: "")
        
        let defaults = UserDefaults.standard
        let data = defaults.string(forKey: "data_thuthach") ?? ""
        let arrThuThach = data.components(separatedBy: "-")
        var didMoved = false
        
        for i in 0..<variable.arrThuThach.count {
            let arr = arrThuThach[i].components(separatedBy: "+")
            variable.arrThuThach[i].doing = Int(arr[0]) ?? 0
            //print("thu thach: \(data)")
            let dateFormatter = DateFormatter()
            dateFormatter.dateFormat = "yyyy_MM_dd"
            if variable.arrThuThach[i].doing == 1 {

                let ngayHienTai = dateFormatter.string(from: Date())
                print("thu thach: \(arr[1])")
                print("thu thach: \(ngayHienTai)")
                
                if (arr[1] == ngayHienTai) {
                    variable.arrThuThach[i].percentToday = Int(arr[2]) ?? 0
                    print("cung ngay: \(arrThuThach[i])")
                  }else{
                      variable.arrThuThach[i].percentToday = 0
                      
                      print("khac ngay")
                  }
                
                variable.arrThuThach[i].ngayTapGanNhat = arr[1]
                
                variable.arrThuThach[i].percentAll = Int(arr[3]) ?? 0
                
                didMoved = Huy_Functions().moveElementAtIndexToFront(array: &variable.arrThuThach, index: i)
            }
        }
        
        
       createTemplate()
       showWarningHaveToWarmupFirst()
       
    }
   func showWarningHaveToWarmupFirst() {
      if(huy_functions.hasNewDayFitness() == true) {
         DispatchQueue.main.asyncAfter(deadline: .now() + 2.0) { // Change `2.0` to the desired SwiftMessages.hide()
            let success = MessageView.viewFromNib(layout: .cardView)
            success.configureTheme(.warning)
            
            success.configureDropShadow()
            success.configureContent(title: "", body: NSLocalizedString("KHOIDONG", comment: ""))
            //success.button?.isHidden = true
            success.button?.setTitle("OK", for: .normal)
            var successConfig = SwiftMessages.defaultConfig
            successConfig.presentationStyle = .bottom
            success.configureTheme(backgroundColor: self.variable.bgColor, foregroundColor: UIColor.white, iconImage: UIImage(named: "warning-white"), iconText: nil)
            success.buttonTapHandler = { _ in
               SwiftMessages.hide()
            }
            successConfig.duration = .seconds(seconds: 20)
            successConfig.dimMode = .gray(interactive: true)
            successConfig.presentationContext = .window(windowLevel: UIWindow.Level.normal)
            SwiftMessages.show(config: successConfig, view: success)
         }
      }
   }
   
    let myScroll:UIScrollView = {
        let v:UIScrollView = UIScrollView()
        v.backgroundColor = #colorLiteral(red: 1, green: 1, blue: 1, alpha: 1)
        v.translatesAutoresizingMaskIntoConstraints = false
        return v
    }()
  
    
    func setBackImage() {
        let yourBackImage = UIImage(named: "prev")
        self.navigationController?.navigationBar.backIndicatorImage = yourBackImage
        self.navigationController?.navigationBar.backIndicatorTransitionMaskImage = yourBackImage
        self.navigationItem.backBarButtonItem = UIBarButtonItem(title: "", style: UIBarButtonItem.Style.plain, target: nil, action: nil)
 
    }
    func createTemplate() {
       
        // add the scroll view to self.view
        let screensize: CGRect = UIScreen.main.bounds
        let screenWidth = screensize.width
        view.addSubview(myScroll)
        myScroll.topAnchor.constraint(equalTo: view.topAnchor).isActive = true
        myScroll.bottomAnchor.constraint(equalTo: view.bottomAnchor).isActive = true
        myScroll.leadingAnchor.constraint(equalTo: view.leadingAnchor).isActive = true
        myScroll.trailingAnchor.constraint(equalTo: view.trailingAnchor).isActive = true
        //myScroll.backgroundColor = .cyan
        myScroll.contentSize = CGSize(width: screenWidth, height: 1100)
        myScroll.showsVerticalScrollIndicator = false
        
        //=======================thu thach=======================================
        if isTapTheoThuThach {
            if let item = variable.arrThuThach.first(where: { $0.key == thuThachDangTap }) {
                myScroll.addSubview(titleThuThach)
                titleThuThach.text = item.name
                titleThuThach.textColor = variable.textBlack
                titleThuThach.font = UIFont(name: "SourceSansPro-Bold", size: 20)
                titleThuThach.topAnchor.constraint(equalTo: myScroll.topAnchor, constant: 10).isActive = true
                titleThuThach.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
                titleThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
                titleThuThach.heightAnchor.constraint(equalToConstant: 25).isActive = true

                myScroll.addSubview(wrapDescThuThach)
                
                wrapDescThuThach.backgroundColor = variable.circleActive
                wrapDescThuThach.layer.cornerRadius = 12
                wrapDescThuThach.topAnchor.constraint(equalTo: myScroll.topAnchor, constant: 5).isActive = true
                wrapDescThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
                wrapDescThuThach.heightAnchor.constraint(equalToConstant: 30).isActive = true

                wrapDescThuThach.addSubview(descThuThach)
                descThuThach.text = NSLocalizedString("THUTHACH", comment: "")
                descThuThach.textColor = .white
                descThuThach.font = UIFont(name: "SourceSansPro-Bold", size: 16)
                descThuThach.topAnchor.constraint(equalTo: wrapDescThuThach.topAnchor, constant: 5).isActive = true
                descThuThach.bottomAnchor.constraint(equalTo: wrapDescThuThach.bottomAnchor, constant: -5).isActive = true
                descThuThach.leadingAnchor.constraint(equalTo: wrapDescThuThach.leadingAnchor, constant: 10).isActive = true
                descThuThach.trailingAnchor.constraint(equalTo: wrapDescThuThach.trailingAnchor, constant: -10).isActive = true
                
                myScroll.addSubview(wrapContentThuThach)
                wrapContentThuThach.backgroundColor = variable.bgHongNhat3
                wrapContentThuThach.layer.cornerRadius = variable.borderRadius3
                wrapDescThuThach.translatesAutoresizingMaskIntoConstraints = false
                wrapContentThuThach.topAnchor.constraint(equalTo: titleThuThach.bottomAnchor, constant: 10).isActive = true
                wrapContentThuThach.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
                wrapContentThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
                wrapContentThuThach.heightAnchor.constraint(equalToConstant: 200).isActive = true
                
                wrapContentThuThach.addSubview(wrapContentThuThachLeft)
                wrapContentThuThachLeft.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
                wrapContentThuThachLeft.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
                wrapContentThuThachLeft.widthAnchor.constraint(equalTo: wrapContentThuThach.widthAnchor, multiplier: 0.5).isActive = true
                wrapContentThuThachLeft.leadingAnchor.constraint(equalTo: wrapContentThuThach.leadingAnchor).isActive = true
                //wrapContentThuThachLeft.backgroundColor = .cyan
                
                wrapContentThuThach.addSubview(wrapContentThuThachRight)
                wrapContentThuThachRight.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
                wrapContentThuThachRight.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
                wrapContentThuThachRight.widthAnchor.constraint(equalTo: wrapContentThuThach.widthAnchor, multiplier: 0.5).isActive = true
                wrapContentThuThachRight.trailingAnchor.constraint(equalTo: wrapContentThuThach.trailingAnchor).isActive = true
                //wrapContentThuThachRight.backgroundColor = .cyan

                // Tạo một UIHostingController với SwiftUI view
                let contentThuthachCricleLeftABC = MyCircleView(isToday: true, isThuThach: true, percentThuthach1Ngay: Double(item.percentToday) / 100, percentThuthachAll: Double(item.percentAll) / 100)
                let contentThuthachCricleLeft = UIHostingController(rootView: contentThuthachCricleLeftABC)

                // Thêm hosting controller vào view controller hiện tại
                addChild(contentThuthachCricleLeft)
                contentThuthachCricleLeft.view.translatesAutoresizingMaskIntoConstraints = false
                wrapContentThuThachLeft.addSubview(contentThuthachCricleLeft.view)

                // Đặt constraints cho hosting controller
                contentThuthachCricleLeft.view.centerXAnchor.constraint(equalTo: wrapContentThuThachLeft.centerXAnchor).isActive = true
                contentThuthachCricleLeft.view.centerYAnchor.constraint(equalTo: wrapContentThuThachLeft.centerYAnchor).isActive = true
                
                contentThuthachCricleLeft.view.heightAnchor.constraint(equalTo: wrapContentThuThachLeft.heightAnchor, multiplier: 0.6).isActive = true
                contentThuthachCricleLeft.view.backgroundColor = UIColor.clear
                contentThuthachCricleLeft.view.widthAnchor.constraint(equalTo: contentThuthachCricleLeft.view.heightAnchor, multiplier: 1).isActive = true

                contentThuthachCricleLeft.didMove(toParent: self)
                
                wrapContentThuThachLeft.addSubview(contentThuthachTitleLeft)
                contentThuthachTitleLeft.centerXAnchor.constraint(equalTo: contentThuthachCricleLeft.view.centerXAnchor).isActive = true
                contentThuthachTitleLeft.topAnchor.constraint(equalTo: contentThuthachCricleLeft.view.bottomAnchor, constant: 5).isActive = true
                contentThuthachTitleLeft.heightAnchor.constraint(equalToConstant: 24).isActive = true
                contentThuthachTitleLeft.text = NSLocalizedString("TODAY", comment: "")
                contentThuthachTitleLeft.textColor = variable.circleText
                contentThuthachTitleLeft.font = UIFont(name: "SourceSansPro-Bold", size: 20)
                
                let wrapThoiGian1 = UIView()
                wrapThoiGian1.translatesAutoresizingMaskIntoConstraints = false
                wrapContentThuThachLeft.addSubview(wrapThoiGian1)
                
                wrapThoiGian1.backgroundColor = variable.circleActive
                wrapThoiGian1.layer.cornerRadius = 12
                wrapThoiGian1.centerXAnchor.constraint(equalTo: contentThuthachCricleLeft.view.centerXAnchor).isActive = true
                wrapThoiGian1.bottomAnchor.constraint(equalTo: contentThuthachCricleLeft.view.topAnchor, constant: -5).isActive = true
                //wrapThoiGian1.widthAnchor.constraint(equalToConstant: 60).isActive = true
                //wrapThoiGian1.heightAnchor.constraint(equalToConstant: 22).isActive = true

                let thoiGian1 = UILabel()
                thoiGian1.translatesAutoresizingMaskIntoConstraints = false
                wrapThoiGian1.addSubview(thoiGian1)
                thoiGian1.text = String(item.time1Day) + " " + NSLocalizedString(item.type, comment: "")
                if item.type == "lientiep" {
                    thoiGian1.isHidden = true
                    wrapThoiGian1.isHidden = true
                }
                thoiGian1.textColor = .white
                thoiGian1.font = UIFont(name: "SourceSansPro-Bold", size: 16)
                thoiGian1.topAnchor.constraint(equalTo: wrapThoiGian1.topAnchor, constant: 1).isActive = true
                thoiGian1.bottomAnchor.constraint(equalTo: wrapThoiGian1.bottomAnchor, constant: -1).isActive = true
                //thoiGian1.centerXAnchor.constraint(equalTo: wrapThoiGian1.centerXAnchor).isActive = true
                thoiGian1.leadingAnchor.constraint(equalTo: wrapThoiGian1.leadingAnchor, constant: 10).isActive = true
                thoiGian1.trailingAnchor.constraint(equalTo: wrapThoiGian1.trailingAnchor, constant: -10).isActive = true
                
                wrapContentThuThach.addSubview(wrapContentThuThachRight)
                wrapContentThuThachRight.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
                wrapContentThuThachRight.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
                
                wrapContentThuThachRight.trailingAnchor.constraint(equalTo: wrapContentThuThach.trailingAnchor).isActive = true
                //wrapContentThuThachRight.backgroundColor = .cyan

                // Tạo một UIHostingController với SwiftUI view
                let contentThuthachCricleLeftABC2 = MyCircleView(isToday: false, isThuThach: true, percentThuthach1Ngay: Double(item.percentToday) / 100, percentThuthachAll: Double(item.percentAll) / 100)
                let contentThuthachCricleRight = UIHostingController(rootView: contentThuthachCricleLeftABC2)

                // Thêm hosting controller vào view controller hiện tại
                addChild(contentThuthachCricleRight)
                contentThuthachCricleRight.view.translatesAutoresizingMaskIntoConstraints = false
                wrapContentThuThachRight.addSubview(contentThuthachCricleRight.view)

                // Đặt constraints cho hosting controller
                contentThuthachCricleRight.view.centerXAnchor.constraint(equalTo: wrapContentThuThachRight.centerXAnchor).isActive = true
                contentThuthachCricleRight.view.centerYAnchor.constraint(equalTo: wrapContentThuThachRight.centerYAnchor).isActive = true
                
                contentThuthachCricleRight.view.heightAnchor.constraint(equalTo: wrapContentThuThachRight.heightAnchor, multiplier: 0.6).isActive = true
                contentThuthachCricleRight.view.backgroundColor = UIColor.clear
                contentThuthachCricleRight.view.widthAnchor.constraint(equalTo: contentThuthachCricleRight.view.heightAnchor, multiplier: 1).isActive = true

                contentThuthachCricleRight.didMove(toParent: self)
                
                wrapContentThuThachRight.addSubview(contentThuthachTitleRight)
                contentThuthachTitleRight.centerXAnchor.constraint(equalTo: contentThuthachCricleRight.view.centerXAnchor).isActive = true
                contentThuthachTitleRight.topAnchor.constraint(equalTo: contentThuthachCricleRight.view.bottomAnchor, constant: 5).isActive = true
                contentThuthachTitleRight.heightAnchor.constraint(equalToConstant: 24).isActive = true
                contentThuthachTitleRight.text = NSLocalizedString("ALL", comment: "")
                contentThuthachTitleRight.textColor = variable.circleText
                contentThuthachTitleRight.font = UIFont(name: "SourceSansPro-Bold", size: 20)
                
                let wrapThoiGian2 = UIView()
                wrapThoiGian2.translatesAutoresizingMaskIntoConstraints = false
                wrapContentThuThachRight.addSubview(wrapThoiGian2)
                
                wrapThoiGian2.backgroundColor = variable.circleActive
                wrapThoiGian2.layer.cornerRadius = 12
                wrapThoiGian2.centerXAnchor.constraint(equalTo: contentThuthachCricleRight.view.centerXAnchor).isActive = true
                wrapThoiGian2.bottomAnchor.constraint(equalTo: contentThuthachCricleRight.view.topAnchor, constant: -5).isActive = true
                //wrapThoiGian2.widthAnchor.constraint(equalToConstant: 60).isActive = true
                //wrapThoiGian2.heightAnchor.constraint(equalToConstant: 22).isActive = true

                let thoiGian2 = UILabel()
                thoiGian2.translatesAutoresizingMaskIntoConstraints = false
                wrapThoiGian2.addSubview(thoiGian2)
                thoiGian2.text = "\(item.timeAll)" + " " + NSLocalizedString("days", comment: "")
                thoiGian2.textColor = .white
                thoiGian2.font = UIFont(name: "SourceSansPro-Bold", size: 16)
                thoiGian2.topAnchor.constraint(equalTo: wrapThoiGian2.topAnchor, constant: 1).isActive = true
                thoiGian2.bottomAnchor.constraint(equalTo: wrapThoiGian2.bottomAnchor, constant: -1).isActive = true
                //thoiGian2.centerXAnchor.constraint(equalTo: wrapThoiGian2.centerXAnchor).isActive = true
                thoiGian2.leadingAnchor.constraint(equalTo: wrapThoiGian2.leadingAnchor, constant: 10).isActive = true
                thoiGian2.trailingAnchor.constraint(equalTo: wrapThoiGian2.trailingAnchor, constant: -10).isActive = true
            }
            
        }
        
        //=======================Muc tieu=======================================
        if isTapTheoMucTieu {
           
            myScroll.addSubview(titleThuThach)
            titleThuThach.text = NSLocalizedString("muctieu\(mucTieuActive)", comment: "")
            let canNangBanDauDonVi = UserDefaults.standard.integer(forKey: "cannangbandaudonvi")
            var canNangBanDauDonViString = "kg"
            if(canNangBanDauDonVi == 1) {
               canNangBanDauDonViString = "lbs"
            }
            
            if canNangBanDauDonViString == "lbs"{
                titleThuThach.text = NSLocalizedString("muctieu\(mucTieuActive)USA", comment: "")
            }
            
            titleThuThach.textColor = variable.textBlack
            titleThuThach.font = UIFont(name: "SourceSansPro-Bold", size: 20)
            titleThuThach.topAnchor.constraint(equalTo: myScroll.topAnchor, constant: 10).isActive = true
            titleThuThach.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
            titleThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
            titleThuThach.heightAnchor.constraint(equalToConstant: 25).isActive = true

            myScroll.addSubview(wrapDescThuThach)
            
            wrapDescThuThach.backgroundColor = variable.circleActive
            wrapDescThuThach.layer.cornerRadius = 12
            wrapDescThuThach.topAnchor.constraint(equalTo: myScroll.topAnchor, constant: 5).isActive = true
            
            wrapDescThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
            wrapDescThuThach.heightAnchor.constraint(equalToConstant: 30).isActive = true

            wrapDescThuThach.addSubview(descThuThach)
            descThuThach.text = "\(variable.mucTieuTime[mucTieuActive - 1])"  + " " + NSLocalizedString("days", comment: "")
            descThuThach.textColor = .white
            descThuThach.font = UIFont(name: "SourceSansPro-Bold", size: 16)
            descThuThach.topAnchor.constraint(equalTo: wrapDescThuThach.topAnchor, constant: 5).isActive = true
            descThuThach.bottomAnchor.constraint(equalTo: wrapDescThuThach.bottomAnchor, constant: -5).isActive = true
            descThuThach.leadingAnchor.constraint(equalTo: wrapDescThuThach.leadingAnchor, constant: 10).isActive = true
            descThuThach.trailingAnchor.constraint(equalTo: wrapDescThuThach.trailingAnchor, constant: -10).isActive = true
            
            myScroll.addSubview(wrapContentThuThach)
            wrapContentThuThach.backgroundColor = variable.bgHongNhat3
            wrapContentThuThach.layer.cornerRadius = variable.borderRadius3
            wrapContentThuThach.translatesAutoresizingMaskIntoConstraints = false
            wrapContentThuThach.topAnchor.constraint(equalTo: titleThuThach.bottomAnchor, constant: 10).isActive = true
            wrapContentThuThach.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
            wrapContentThuThach.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
            wrapContentThuThach.heightAnchor.constraint(equalToConstant: 200).isActive = true
            
            wrapContentThuThach.addSubview(wrapContentThuThachLeft)
            wrapContentThuThachLeft.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
            wrapContentThuThachLeft.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
            wrapContentThuThachLeft.widthAnchor.constraint(equalTo: wrapContentThuThach.widthAnchor, multiplier: 0.5).isActive = true
            wrapContentThuThachLeft.leadingAnchor.constraint(equalTo: wrapContentThuThach.leadingAnchor).isActive = true
            //wrapContentThuThachLeft.backgroundColor = .cyan
            
            wrapContentThuThach.addSubview(wrapContentThuThachRight)
            wrapContentThuThachRight.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
            wrapContentThuThachRight.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
            wrapContentThuThachRight.widthAnchor.constraint(equalTo: wrapContentThuThach.widthAnchor, multiplier: 0.5).isActive = true
            wrapContentThuThachRight.trailingAnchor.constraint(equalTo: wrapContentThuThach.trailingAnchor).isActive = true
            //wrapContentThuThachRight.backgroundColor = .yellow

            // Tạo một UIHostingController với SwiftUI view
            let contentThuthachCricleLeftABC = MyCircleView(isToday: true, isThuThach: false)
            let contentThuthachCricleLeft = UIHostingController(rootView: contentThuthachCricleLeftABC)

            // Thêm hosting controller vào view controller hiện tại
            addChild(contentThuthachCricleLeft)
            contentThuthachCricleLeft.view.translatesAutoresizingMaskIntoConstraints = false
            wrapContentThuThachLeft.addSubview(contentThuthachCricleLeft.view)

            // Đặt constraints cho hosting controller
            contentThuthachCricleLeft.view.centerXAnchor.constraint(equalTo: wrapContentThuThachLeft.centerXAnchor).isActive = true
            contentThuthachCricleLeft.view.centerYAnchor.constraint(equalTo: wrapContentThuThachLeft.centerYAnchor).isActive = true
            
            contentThuthachCricleLeft.view.heightAnchor.constraint(equalTo: wrapContentThuThachLeft.heightAnchor, multiplier: 0.6).isActive = true
            contentThuthachCricleLeft.view.backgroundColor = UIColor.clear
            contentThuthachCricleLeft.view.widthAnchor.constraint(equalTo: contentThuthachCricleLeft.view.heightAnchor, multiplier: 1).isActive = true

            contentThuthachCricleLeft.didMove(toParent: self)
            
            wrapContentThuThachLeft.addSubview(contentThuthachTitleLeft)
            contentThuthachTitleLeft.centerXAnchor.constraint(equalTo: contentThuthachCricleLeft.view.centerXAnchor).isActive = true
            contentThuthachTitleLeft.topAnchor.constraint(equalTo: contentThuthachCricleLeft.view.bottomAnchor, constant: 5).isActive = true
            contentThuthachTitleLeft.heightAnchor.constraint(equalToConstant: 24).isActive = true
            contentThuthachTitleLeft.text = NSLocalizedString("TODAY", comment: "")
            contentThuthachTitleLeft.textColor = variable.circleText
            contentThuthachTitleLeft.font = UIFont(name: "SourceSansPro-Bold", size: 20)
            
            let wrapThoiGian1 = UIView()
            wrapThoiGian1.translatesAutoresizingMaskIntoConstraints = false
            wrapContentThuThachLeft.addSubview(wrapThoiGian1)
            
            wrapThoiGian1.backgroundColor = variable.circleActive
            wrapThoiGian1.layer.cornerRadius = 12
            wrapThoiGian1.centerXAnchor.constraint(equalTo: contentThuthachCricleLeft.view.centerXAnchor).isActive = true
            wrapThoiGian1.bottomAnchor.constraint(equalTo: contentThuthachCricleLeft.view.topAnchor, constant: -5).isActive = true
            //wrapThoiGian1.widthAnchor.constraint(equalToConstant: 60).isActive = true
            //wrapThoiGian1.heightAnchor.constraint(equalToConstant: 22).isActive = true

            let thoiGian1 = UILabel()
            thoiGian1.translatesAutoresizingMaskIntoConstraints = false
            wrapThoiGian1.addSubview(thoiGian1)
            thoiGian1.text = String(caloCanTapHomNay) + " kcal"
            thoiGian1.textColor = .white
            thoiGian1.font = UIFont(name: "SourceSansPro-Bold", size: 16)
            thoiGian1.topAnchor.constraint(equalTo: wrapThoiGian1.topAnchor, constant: 1).isActive = true
            thoiGian1.bottomAnchor.constraint(equalTo: wrapThoiGian1.bottomAnchor, constant: -1).isActive = true
            //thoiGian1.centerXAnchor.constraint(equalTo: wrapThoiGian1.centerXAnchor).isActive = true
            thoiGian1.leadingAnchor.constraint(equalTo: wrapThoiGian1.leadingAnchor, constant: 10).isActive = true
            thoiGian1.trailingAnchor.constraint(equalTo: wrapThoiGian1.trailingAnchor, constant: -10).isActive = true
            
            wrapContentThuThach.addSubview(wrapContentThuThachRight)
            wrapContentThuThachRight.topAnchor.constraint(equalTo: wrapContentThuThach.topAnchor).isActive = true
            wrapContentThuThachRight.bottomAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor).isActive = true
            
            wrapContentThuThachRight.trailingAnchor.constraint(equalTo: wrapContentThuThach.trailingAnchor).isActive = true
            //wrapContentThuThachRight.backgroundColor = .cyan

            // Tạo một UIHostingController với SwiftUI view
            let contentThuthachCricleLeftABC2 = MyCircleView(isToday: false, isThuThach: false)
            let contentThuthachCricleRight = UIHostingController(rootView: contentThuthachCricleLeftABC2)

            // Thêm hosting controller vào view controller hiện tại
            addChild(contentThuthachCricleRight)
            contentThuthachCricleRight.view.translatesAutoresizingMaskIntoConstraints = false
            wrapContentThuThachRight.addSubview(contentThuthachCricleRight.view)

            // Đặt constraints cho hosting controller
            contentThuthachCricleRight.view.centerXAnchor.constraint(equalTo: wrapContentThuThachRight.centerXAnchor).isActive = true
            contentThuthachCricleRight.view.centerYAnchor.constraint(equalTo: wrapContentThuThachRight.centerYAnchor).isActive = true
            
            contentThuthachCricleRight.view.heightAnchor.constraint(equalTo: wrapContentThuThachRight.heightAnchor, multiplier: 0.6).isActive = true
            contentThuthachCricleRight.view.backgroundColor = .clear
            contentThuthachCricleRight.view.widthAnchor.constraint(equalTo: contentThuthachCricleRight.view.heightAnchor, multiplier: 1).isActive = true

            contentThuthachCricleRight.didMove(toParent: self)
            
            wrapContentThuThachRight.addSubview(contentThuthachTitleRight)
            contentThuthachTitleRight.centerXAnchor.constraint(equalTo: contentThuthachCricleRight.view.centerXAnchor).isActive = true
            contentThuthachTitleRight.topAnchor.constraint(equalTo: contentThuthachCricleRight.view.bottomAnchor, constant: 5).isActive = true
            contentThuthachTitleRight.heightAnchor.constraint(equalToConstant: 24).isActive = true
            contentThuthachTitleRight.text = NSLocalizedString("ALL", comment: "")
            contentThuthachTitleRight.textColor = variable.circleText
            contentThuthachTitleRight.font = UIFont(name: "SourceSansPro-Bold", size: 20)
            
            let wrapThoiGian2 = UIView()
            wrapThoiGian2.translatesAutoresizingMaskIntoConstraints = false
            wrapContentThuThachRight.addSubview(wrapThoiGian2)
            
            wrapThoiGian2.backgroundColor = variable.circleActive
            wrapThoiGian2.layer.cornerRadius = 12
            wrapThoiGian2.centerXAnchor.constraint(equalTo: contentThuthachCricleRight.view.centerXAnchor).isActive = true
            wrapThoiGian2.bottomAnchor.constraint(equalTo: contentThuthachCricleRight.view.topAnchor, constant: -5).isActive = true
            //wrapThoiGian2.widthAnchor.constraint(equalToConstant: 60).isActive = true
            //wrapThoiGian2.heightAnchor.constraint(equalToConstant: 22).isActive = true

            let thoiGian2 = UILabel()
            thoiGian2.translatesAutoresizingMaskIntoConstraints = false
            wrapThoiGian2.addSubview(thoiGian2)
            thoiGian2.text = String(variable.arrMucTieuCalo[mucTieuActive - 1]) + " kcal"
            thoiGian2.textColor = .white
            thoiGian2.font = UIFont(name: "SourceSansPro-Bold", size: 16)
            thoiGian2.topAnchor.constraint(equalTo: wrapThoiGian2.topAnchor, constant: 1).isActive = true
            thoiGian2.bottomAnchor.constraint(equalTo: wrapThoiGian2.bottomAnchor, constant: -1).isActive = true
            //thoiGian2.centerXAnchor.constraint(equalTo: wrapThoiGian2.centerXAnchor).isActive = true
            thoiGian2.leadingAnchor.constraint(equalTo: wrapThoiGian2.leadingAnchor, constant: 10).isActive = true
            thoiGian2.trailingAnchor.constraint(equalTo: wrapThoiGian2.trailingAnchor, constant: -10).isActive = true
        }
        
//        var withItem: CGFloat = 0.2
//        var fixSmallWidthIpad:CGFloat = 1
//        if UIDevice.current.userInterfaceIdiom == .pad   {
//            withItem = 0.12
//            fixSmallWidthIpad = 0.8
//            
//        }
//        if traitCollection.horizontalSizeClass == .regular &&  UIDevice.current.userInterfaceIdiom != .pad{
//            withItem = 0.1
//            fixSmallWidthIpad = 1.2
//        }
//        if traitCollection.horizontalSizeClass == .compact &&  UIDevice.current.userInterfaceIdiom != .pad {
//            withItem = 0.2
//            fixSmallWidthIpad = 1
//        }
        
        //=======================cac bai tap====================================
        if let item = variable.arrThuThach.first(where: { $0.key == thuThachDangTap }) {
            if isTapTheoThuThach && item.loaiBaiTap != "all" {
                let topTitle =  15
                let titleBaiTap = UILabel()
                titleBaiTap.translatesAutoresizingMaskIntoConstraints = false
                myScroll.addSubview(titleBaiTap)
                titleBaiTap.text = NSLocalizedString("\(item.loaiBaiTap)", comment: "")
                titleBaiTap.textColor = variable.textBlack
                titleBaiTap.font = UIFont(name: "SourceSansPro-Bold", size: 18)
                
                if isTapTheoThuThach || isTapTheoMucTieu {
                    titleBaiTap.topAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor, constant: CGFloat(topTitle)).isActive = true
                } else {
                    titleBaiTap.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: CGFloat(topTitle)).isActive = true
                }
                
                titleBaiTap.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
                titleBaiTap.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
                titleBaiTap.heightAnchor.constraint(equalToConstant: 30).isActive = true
                
                let wrapBaiTap = UIView()
                wrapBaiTap.translatesAutoresizingMaskIntoConstraints = false
                myScroll.addSubview(wrapBaiTap)
                wrapBaiTap.topAnchor.constraint(equalTo: titleBaiTap.bottomAnchor, constant: 5).isActive = true
                wrapBaiTap.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 20).isActive = true
                wrapBaiTap.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -20).isActive = true
                wrapBaiTap.heightAnchor.constraint(equalToConstant: 120).isActive = true
                wrapBaiTap.backgroundColor = variable.bgHongNhat3
                wrapBaiTap.layer.cornerRadius = variable.borderRadius3
                wrapBaiTap.dropShadow(myColor: variable.bgHongNhat)
                
                let childBaiTap = UIView()
                childBaiTap.translatesAutoresizingMaskIntoConstraints = false
                wrapBaiTap.addSubview(childBaiTap)
                baiTapItems.append(childBaiTap)
                childBaiTap.topAnchor.constraint(equalTo: wrapBaiTap.topAnchor, constant: 0).isActive = true
                childBaiTap.bottomAnchor.constraint(equalTo: wrapBaiTap.bottomAnchor, constant: 0).isActive = true
                childBaiTap.leadingAnchor.constraint(equalTo: wrapBaiTap.leadingAnchor, constant: 5).isActive = true
                childBaiTap.trailingAnchor.constraint(equalTo: wrapBaiTap.trailingAnchor, constant: -5).isActive = true
                //childBaiTap.backgroundColor = .cyan
                
                var i = 0
                for j in 0..<5 {
                    var phutTap = 6
                    switch item.loaiBaiTap {
                    case "tapbung":
                        phutTap = variable.absTime[j]
                        i = 0
                    case "tapmong":
                        phutTap = variable.buttTime[j]
                        i = 1
                    case "tapchan":
                        phutTap = variable.legTime[j]
                        i = 2
                    case "taptay":
                        phutTap = variable.armTime[j]
                        i = 3
                    case "tapnguc":
                        phutTap = variable.chestTime[j]
                        i = 4
                    default:
                        phutTap = variable.absTime[j]
                    }
                    let caloTap = phutTap * variable.cal[i]
                    
                    let wrapChild1 = UIView()
                    //let leftChild1 = CGFloat(j) * (screenWidth - 40 - 10) * 0.2
                    wrapChild1.translatesAutoresizingMaskIntoConstraints = false
                    childBaiTap.addSubview(wrapChild1)
                    itemViews.append(wrapChild1)
                    wrapChild1.centerYAnchor.constraint(equalTo: childBaiTap.centerYAnchor, constant: 0).isActive = true
//                    wrapChild1.leadingAnchor.constraint(equalTo: childBaiTap.leadingAnchor, constant: leftChild1).isActive = true
//                    wrapChild1.widthAnchor.constraint(equalTo: childBaiTap.widthAnchor, multiplier: withItem).isActive = true
                    wrapChild1.heightAnchor.constraint(equalToConstant: 90).isActive = true
                    //wrapChild1.backgroundColor = .cyan
                   
                    let wrapChild = UIView()
                    
                    wrapChild.translatesAutoresizingMaskIntoConstraints = false
                    wrapChild1.addSubview(wrapChild)
                    wrapChild.topAnchor.constraint(equalTo: wrapChild1.topAnchor, constant: 0).isActive = true
                    wrapChild.bottomAnchor.constraint(equalTo: wrapChild1.bottomAnchor, constant: 0).isActive = true
                    wrapChild.leadingAnchor.constraint(equalTo: wrapChild1.leadingAnchor, constant: 5).isActive = true
                    wrapChild.trailingAnchor.constraint(equalTo: wrapChild1.trailingAnchor, constant: -5).isActive = true
                    
                    wrapChild.backgroundColor = variable.colorItemWorkout
                   
                    //wrapChild.layer.cornerRadius = withItem * fixSmallWidthIpad * (screenWidth - 100) / 2

                    wrapChild.isUserInteractionEnabled = true
                    let tapGesture = UITapGestureRecognizer(target: self, action: #selector(playAction(sender:)))
                    wrapChild.tag = (i + 1) * 10 + (j + 1)
                    //print ("click \(wrapChild.tag)")
                    wrapChild.addGestureRecognizer(tapGesture)
                    
                    
                    let label1 = UILabel()
                    label1.translatesAutoresizingMaskIntoConstraints = false
                    wrapChild.addSubview(label1)
                    label1.text = "\(phutTap)"
                    label1.textColor = variable.textBlack
                    label1.font = UIFont(name: "SourceSansPro-Bold", size: 25)
                    label1.topAnchor.constraint(equalTo: wrapChild.topAnchor, constant: 5).isActive = true
                    label1.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                    
                    label1.heightAnchor.constraint(equalToConstant: 18).isActive = true
                    
                    let label2 = UILabel()
                    label2.translatesAutoresizingMaskIntoConstraints = false
                    wrapChild.addSubview(label2)
                    label2.text = NSLocalizedString("PHUT", comment: "")
                    label2.textColor = variable.textBlack
                    label2.font = UIFont(name: "SourceSansPro-Bold", size: 11)
                    label2.topAnchor.constraint(equalTo: label1.bottomAnchor, constant: 0).isActive = true
                    label2.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                    label2.heightAnchor.constraint(equalToConstant: 20).isActive = true
                    
                    let label3 = UILabel()
                    label3.translatesAutoresizingMaskIntoConstraints = false
                    wrapChild.addSubview(label3)
                    label3.text = NSLocalizedString("kcal", comment: "")
                    label3.textColor = variable.textBlack
                    label3.font = UIFont(name: "SourceSansPro-Bold", size: 11)
                    label3.bottomAnchor.constraint(equalTo: wrapChild.bottomAnchor, constant: -5).isActive = true
                    label3.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                    label3.heightAnchor.constraint(equalToConstant: 15).isActive = true
                    
                    let label4 = UILabel()
                    label4.translatesAutoresizingMaskIntoConstraints = false
                    wrapChild.addSubview(label4)
                    label4.text = "\(caloTap)"
                    label4.textColor = variable.textBlack
                    label4.font = UIFont(name: "SourceSansPro-Bold", size: 20)
                    label4.bottomAnchor.constraint(equalTo: label3.topAnchor, constant: 0).isActive = true
                    label4.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                    label4.heightAnchor.constraint(equalToConstant: 17).isActive = true
                    
                    if variable.workoutActive[i] == j {
                        label1.textColor = .white
                        label2.textColor = .white
                        label3.textColor = .white
                        label4.textColor = .white
                        wrapChild.backgroundColor = variable.circleActive
                    }
                }
                let img = UIImageView()
                img.translatesAutoresizingMaskIntoConstraints = false
                img.image = UIImage(named: variable.workoutWrapImage[i])
                img.contentMode = .scaleAspectFit
                wrapBaiTap.addSubview(img)
                img.topAnchor.constraint(equalTo: wrapBaiTap.topAnchor, constant: -30).isActive = true
                img.trailingAnchor.constraint(equalTo: wrapBaiTap.trailingAnchor, constant: -52).isActive = true
                img.widthAnchor.constraint(equalToConstant: 60).isActive = true
                img.heightAnchor.constraint(equalTo: img.widthAnchor).isActive = true
                img.layer.cornerRadius = 30
                img.dropShadow(myColor: variable.bgHongNhat)
                img.layer.borderWidth = 2
                img.layer.borderColor = variable.colorHongText.cgColor
                img.backgroundColor = .white
            } else {
                for i in 0..<5 {
                    let topTitle = i * 165 + 15
                    let titleBaiTap = UILabel()
                    titleBaiTap.translatesAutoresizingMaskIntoConstraints = false
                    myScroll.addSubview(titleBaiTap)
                    titleBaiTap.text = NSLocalizedString("\(variable.workoutName[i])", comment: "")
                    titleBaiTap.textColor = variable.textBlack
                    titleBaiTap.font = UIFont(name: "SourceSansPro-Bold", size: 18)
                    
                    if isTapTheoThuThach || isTapTheoMucTieu {
                        titleBaiTap.topAnchor.constraint(equalTo: wrapContentThuThach.bottomAnchor, constant: CGFloat(topTitle)).isActive = true
                    } else {
                        titleBaiTap.topAnchor.constraint(equalTo: myScroll.topAnchor, constant: CGFloat(topTitle)).isActive = true
                        
                    }
                    
                    titleBaiTap.leadingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.leadingAnchor, constant: 20).isActive = true
                    titleBaiTap.trailingAnchor.constraint(equalTo: view.safeAreaLayoutGuide.trailingAnchor, constant: -20).isActive = true
                    titleBaiTap.heightAnchor.constraint(equalToConstant: 30).isActive = true
                    
                    let wrapBaiTap = UIView()
                    wrapBaiTap.translatesAutoresizingMaskIntoConstraints = false
                    myScroll.addSubview(wrapBaiTap)
                    wrapBaiTap.topAnchor.constraint(equalTo: titleBaiTap.bottomAnchor, constant: 5).isActive = true
                    wrapBaiTap.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 20).isActive = true
                    wrapBaiTap.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -20).isActive = true
                    wrapBaiTap.heightAnchor.constraint(equalToConstant: 120).isActive = true
                    wrapBaiTap.backgroundColor = variable.bgHongNhat3
                    wrapBaiTap.layer.cornerRadius = variable.borderRadius3
                    wrapBaiTap.dropShadow(myColor: variable.bgHongNhat)
                    
                    let childBaiTap = UIView()
                    childBaiTap.translatesAutoresizingMaskIntoConstraints = false
                    wrapBaiTap.addSubview(childBaiTap)
                    childBaiTap.topAnchor.constraint(equalTo: wrapBaiTap.topAnchor, constant: 0).isActive = true
                    childBaiTap.bottomAnchor.constraint(equalTo: wrapBaiTap.bottomAnchor, constant: 0).isActive = true
                    childBaiTap.leadingAnchor.constraint(equalTo: wrapBaiTap.leadingAnchor, constant: 5).isActive = true
                    childBaiTap.trailingAnchor.constraint(equalTo: wrapBaiTap.trailingAnchor, constant: -5).isActive = true
                    //childBaiTap.backgroundColor = .cyan
                    
                    
                    for j in 0..<5 {
                        var phutTap = 6
                        switch i {
                        case 0:
                            phutTap = variable.absTime[j]
                        case 1:
                            phutTap = variable.buttTime[j]
                        case 2:
                            phutTap = variable.legTime[j]
                        case 3:
                            phutTap = variable.armTime[j]
                        case 4:
                            phutTap = variable.chestTime[j]
                        default:
                            phutTap = variable.absTime[j]
                        }
                        let caloTap = phutTap * variable.cal[i]
                        
                        let wrapChild1 = UIView()
                        //let leftChild1 = CGFloat(j) * (screenWidth - 40 - 10) * 0.2 * fixSmallWidthIpad
                        wrapChild1.translatesAutoresizingMaskIntoConstraints = false
                        childBaiTap.addSubview(wrapChild1)
                        wrapChild1.centerYAnchor.constraint(equalTo: childBaiTap.centerYAnchor, constant: 0).isActive = true
//                        wrapChild1.leadingAnchor.constraint(equalTo: childBaiTap.leadingAnchor, constant: leftChild1).isActive = true
//                        wrapChild1.widthAnchor.constraint(equalTo: childBaiTap.widthAnchor, multiplier: withItem).isActive = true
                        wrapChild1.heightAnchor.constraint(equalToConstant: 90).isActive = true
                        //wrapChild1.backgroundColor = .cyan
                       
                        
                        let wrapChild = UIView()
                        
                        wrapChild.translatesAutoresizingMaskIntoConstraints = false
                        wrapChild1.addSubview(wrapChild)
                        wrapChild.topAnchor.constraint(equalTo: wrapChild1.topAnchor, constant: 0).isActive = true
                        wrapChild.bottomAnchor.constraint(equalTo: wrapChild1.bottomAnchor, constant: 0).isActive = true
                        wrapChild.leadingAnchor.constraint(equalTo: wrapChild1.leadingAnchor, constant: 5).isActive = true
                        wrapChild.trailingAnchor.constraint(equalTo: wrapChild1.trailingAnchor, constant: -5).isActive = true
                        
                        wrapChild.backgroundColor = variable.colorItemWorkout
                       
                        //wrapChild.layer.cornerRadius = withItem * (screenWidth - 100) / 2
                        

                        wrapChild.isUserInteractionEnabled = true
                        let tapGesture = UITapGestureRecognizer(target: self, action: #selector(playAction(sender:)))
                        wrapChild.tag = (i + 1) * 10 + (j + 1)
                        //print ("click \(wrapChild.tag)")
                        wrapChild.addGestureRecognizer(tapGesture)
                        
                        
                        let label1 = UILabel()
                        label1.translatesAutoresizingMaskIntoConstraints = false
                        wrapChild.addSubview(label1)
                        label1.text = "\(phutTap)"
                        label1.textColor = variable.textBlack
                        label1.font = UIFont(name: "SourceSansPro-Bold", size: 25)
                        label1.topAnchor.constraint(equalTo: wrapChild.topAnchor, constant: 5).isActive = true
                        label1.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                        
                        label1.heightAnchor.constraint(equalToConstant: 18).isActive = true
                        
                        let label2 = UILabel()
                        label2.translatesAutoresizingMaskIntoConstraints = false
                        wrapChild.addSubview(label2)
                        label2.text = NSLocalizedString("PHUT", comment: "")
                        label2.textColor = variable.textBlack
                        label2.font = UIFont(name: "SourceSansPro-Bold", size: 11)
                        label2.topAnchor.constraint(equalTo: label1.bottomAnchor, constant: 0).isActive = true
                        label2.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                        label2.heightAnchor.constraint(equalToConstant: 20).isActive = true
                        
                        let label3 = UILabel()
                        label3.translatesAutoresizingMaskIntoConstraints = false
                        wrapChild.addSubview(label3)
                        label3.text = NSLocalizedString("kcal", comment: "")
                        label3.textColor = variable.textBlack
                        label3.font = UIFont(name: "SourceSansPro-Bold", size: 11)
                        label3.bottomAnchor.constraint(equalTo: wrapChild.bottomAnchor, constant: -5).isActive = true
                        label3.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                        label3.heightAnchor.constraint(equalToConstant: 15).isActive = true
                        
                        let label4 = UILabel()
                        label4.translatesAutoresizingMaskIntoConstraints = false
                        wrapChild.addSubview(label4)
                        label4.text = "\(caloTap)"
                        label4.textColor = variable.textBlack
                        label4.font = UIFont(name: "SourceSansPro-Bold", size: 20)
                        label4.bottomAnchor.constraint(equalTo: label3.topAnchor, constant: 0).isActive = true
                        label4.centerXAnchor.constraint(equalTo: wrapChild.centerXAnchor, constant: 0).isActive = true
                        label4.heightAnchor.constraint(equalToConstant: 17).isActive = true
                        
                        if variable.workoutActive[i] == j {
                            label1.textColor = .white
                            label2.textColor = .white
                            label3.textColor = .white
                            label4.textColor = .white
                            wrapChild.backgroundColor = variable.circleActive
                        }
                    }
                    let img = UIImageView()
                    img.translatesAutoresizingMaskIntoConstraints = false
                    img.image = UIImage(named: variable.workoutWrapImage[i])
                    img.contentMode = .scaleAspectFit
                    wrapBaiTap.addSubview(img)
                    img.topAnchor.constraint(equalTo: wrapBaiTap.topAnchor, constant: -30).isActive = true
                    img.trailingAnchor.constraint(equalTo: wrapBaiTap.trailingAnchor, constant: -52).isActive = true
                    img.widthAnchor.constraint(equalToConstant: 60).isActive = true
                    img.heightAnchor.constraint(equalTo: img.widthAnchor).isActive = true
                    img.layer.cornerRadius = 30
                    img.dropShadow(myColor: variable.bgHongNhat)
                    img.layer.borderWidth = 2
                    img.layer.borderColor = variable.colorHongText.cgColor
                    img.backgroundColor = .white
                }
            }
        }
        updateLayout()
        
   
    }
    @IBAction func playAction(sender: UITapGestureRecognizer) -> Void {
        let button = sender.view
        workoutToday = button?.tag ?? 11
        
        print("clicked workout: \(workoutToday)")
        if workoutToday > 50 {
            Analytics.logEvent("tap_toan_than", parameters: [
                "time": workoutToday - 50
            ])
        } else if workoutToday > 40 {
            Analytics.logEvent("tap_tay", parameters: [
                "time": workoutToday - 40
            ])
        } else if workoutToday > 30 {
            Analytics.logEvent("tap_chan", parameters: [
                "time": workoutToday - 30
            ])
        } else if workoutToday > 20 {
            Analytics.logEvent("tap_mong", parameters: [
                "time": workoutToday - 20
            ])
        } else{
            Analytics.logEvent("tap_bung", parameters: [
                "time": workoutToday - 10
            ])
        }
        
        
        redirectTo(screen: "ListTrain")
    }
    func redirectTo(screen: String) {
        let loginScreen = storyboard?.instantiateViewController(withIdentifier: screen)
        navigationController?.pushViewController(loginScreen!, animated: true)
    }
    
}
