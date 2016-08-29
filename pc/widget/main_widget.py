#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

from PyQt5.QtWidgets import *

from PyQt5.QtCore import pyqtSignal
from PyQt5.uic import loadUiType, loadUi

from widget.analysis_widget import FCAnalysisWidget
from widget.capture_widget import FCCaptureWidget
from widget.pid_widget import FCPidWidget

class FCMainWidget(QTabWidget):
    def __init__(self):
        super(FCMainWidget, self).__init__() 

        # pid分析控件
        self.mPidWidget = FCPidWidget("widget/ui/fc_pidWidget.ui")
        self.addTab(self.mPidWidget, "PID分析")

        # 在线采集控件
        self.mCaptureWidget = FCCaptureWidget()
        self.addTab(self.mCaptureWidget, "在线控制")

        # 离线分析控件
        self.mAnalysisWidget = FCAnalysisWidget()
        self.addTab(self.mAnalysisWidget, "离线分析")

        # 默认数据采集有效
        self.setCurrentIndex(0)

    def closeEvent(self, event):
        self.mPidWidget.close()
        self.mCaptureWidget.close()
        self.mAnalysisWidget.close()

if __name__ == '__main__': 
    app = QApplication(sys.argv)
    win = FCWidget()
    win.show()
    sys.exit(app.exec_())
