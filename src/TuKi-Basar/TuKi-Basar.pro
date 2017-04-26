#-------------------------------------------------
#
# Project created by QtCreator 2014-12-06T14:07:10
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = TuKi-Basar
TEMPLATE = app

SOURCES += main.cpp\
        TuKiBasar.cpp \
    Evaluation.cpp \
    Settings.cpp \
    Article.cpp \
    ArticleManager.cpp \
    PrizeCorrection.cpp \
    EvaluationView.cpp \
    Statistics.cpp \
    Seller.cpp \
    SellerManager.cpp \
    Converter.cpp \
    CssHelper.cpp \
    SalesView.cpp \
    PasswordInput.cpp \
    DateTimeHelper.cpp \
    ExcelExport.cpp \
    ArticleReturn.cpp \
    EvaluationByCategory.cpp \
    PercentageCalculator.cpp

HEADERS  += TuKiBasar.h \
    Evaluation.h \
    Settings.h \
    Article.h \
    ArticleManager.h \
    PrizeCorrection.h \
    EvaluationView.h \
    Statistics.h \
    Seller.h \
    SellerManager.h \
    Converter.h \
    CssHelper.h \
    SalesView.h \
    PasswordInput.h \
    DateTimeHelper.h \
    ExcelExport.h \
    ArticleReturn.h \
    EvaluationByCategory.h \
    PercentageCalculator.h

FORMS    += TuKiBasar.ui \
    Evaluation.ui \
    Settings.ui \
    PrizeCorrection.ui \
    EvaluationView.ui \
    SalesView.ui \
    PasswordInput.ui \
    ExcelExport.ui \
    ArticleReturn.ui \
    EvaluationByCategory.ui

OTHER_FILES += \
    Nodes.md
