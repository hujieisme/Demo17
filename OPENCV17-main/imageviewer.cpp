#include "imageviewer.h"
#include "ui_imageviewer.h"

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QDateTime>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QScrollArea>
#include <QMimeData>
#include <QScrollBar>
//debug
//#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/hujieisme.svg"));
    initUI();
    if(!qApp->arguments().value(1).isEmpty()){
        readFile(qApp->arguments().value(1));
    }
}

ImageViewer::~ImageViewer()
{
    delete ui;
}


void ImageViewer::initUI(){
    initToolBar();
    initStatusBar();

    imgArea = new QLabel();
    imgArea->setAlignment(Qt::AlignCenter);
    IconSize = QSize(210,210);
    CellSize = QSize(210,210);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(imgArea);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    ui->scrollArea->setFocusPolicy(Qt::NoFocus);

    ui->listWidget->setBackgroundRole((QPalette::Highlight));
    ui->listWidget->setFixedWidth(width);
    ui->listWidget->setIconSize(IconSize);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setSpacing(10);
//    ui->listWidget->horizontalScrollBar()->setDisable(true);

    ui->scrollArea_2->setBackgroundRole(QPalette::Highlight);
    ui->scrollArea_2->setAlignment(Qt::AlignCenter);
    ui->scrollArea_2->setFocusPolicy(Qt::NoFocus);
    ui->scrollArea_2->setFixedWidth(width);
//    ui->listWidget->setFixedHeight(ui->scrollArea->height());

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->addWidget(ui->scrollArea_2);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(ui->scrollArea);


//    contentLayout->setMargin(0);

    QWidget *window = new QWidget;
    window->setLayout(contentLayout);

    setAcceptDrops(true);//????????????????????????
    setCentralWidget(window);
    setContextMenuPolicy(Qt::NoContextMenu);//???????????????????????????
    setWindowTitle(tr("????????????"));
    resize(1200, 800);
    show();
}

void ImageViewer::creatListWidget()
{
    ui->listWidget->clear();
    for (int i = 0;i<files.count();i++)
    {
        QPixmap pixmap(fileInfo.absolutePath()+"/"+files.at(i));
        QListWidgetItem *item = new QListWidgetItem;
        QWidget *widget = new QWidget;
        QVBoxLayout *widgetLayout = new QVBoxLayout;
        QLabel *imageLabel = new QLabel;
        QLabel *txtLabel = new QLabel(tr("%1").arg(files.at(i)));
        widget->setLayout(widgetLayout);
//        widgetLayout->setContentsMargins(0,0,0,0);
        widgetLayout->setSpacing(0);
        widgetLayout->addWidget(imageLabel);
        widgetLayout->addWidget(txtLabel);
        if(pixmap.width()>227||pixmap.height()>227)
                {
                    pixmap=pixmap.scaled(234,234,Qt::KeepAspectRatio);
                }
        imageLabel->setPixmap(pixmap);
        txtLabel->setFixedHeight(30);
        txtLabel->setWordWrap(true);
        txtLabel->setAlignment(Qt::AlignHCenter);

        item->setSizeHint(QSize(250,200));
        ui->listWidget->addItem(item);
        ui->listWidget->setSizeIncrement(250,200);
        ui->listWidget->setItemWidget(item,widget);


        ui->listWidget->setStyleSheet(R"(
            QListWidget { outline: none; border:1px solid gray; color: black; }
            QListWidget::Item { width: 50px; height: 50px; }
            QListWidget::Item:hover { background: #4CAF50; color: white; }
            QListWidget::item:selected { background: #e7e7e7; color: #f44336; }
            QListWidget::item:selected:!active { background: lightgreen; }
                            )");

    }
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(on_listWidgetItem(QListWidgetItem*)));
}


void ImageViewer::on_listWidgetItem(QListWidgetItem* )
{
    int num = 0;
    num = ui->listWidget->currentRow();
    readFile(fileInfo.absolutePath()+"/"+files.at(num));
}

//void ImageViewer::initMenubar(){
//    m_menu_bar = new QMenuBar(this);
//    m_menu_bar->setObjectName("Menubar");

//}

void ImageViewer::initToolBar(){
    QToolBar *fileToolBar = addToolBar(tr("?????????"));
    QToolBar *operateToolBar = addToolBar(tr("?????????"));
    addToolBar(Qt::RightToolBarArea,operateToolBar);
//    fileToolBar->setMovable(false);//?????????????????????

    //??????????????????
    const QIcon cropIcon = QIcon::fromTheme("operate-new",QIcon(":/images/corp.svg"));
    QAction *cropAct = new QAction(cropIcon, tr("&??????"), this);
    cropAct->setShortcuts(QKeySequence::New);
    connect(cropAct, &QAction::triggered, this, &ImageViewer::on_selectImageFile_clicked);
    operateToolBar->addAction(cropAct);


    //??????????????????
    const QIcon rotateIcon = QIcon::fromTheme("operate-new",QIcon(":/images/rotate.svg"));
    QAction *rotateAct = new QAction(rotateIcon, tr("&??????"), this);
    rotateAct->setShortcuts(QKeySequence::New);
    connect(rotateAct, &QAction::triggered, this, &ImageViewer::on_selectImageFile_clicked);
    operateToolBar->addAction(rotateAct);

    //??????????????????
    const QIcon sortIcon = QIcon::fromTheme("operate-new",QIcon(":/images/sort.svg"));
    QAction *sortAct = new QAction(sortIcon, tr("&??????"), this);
    sortAct->setShortcuts(QKeySequence::New);
    connect(sortAct, &QAction::triggered, this, &ImageViewer::on_selectImageFile_clicked);
    operateToolBar->addAction(sortAct);

    const QIcon openImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/OpenImage.svg"));
    QAction *openImageAct = new QAction(openImageIcon, tr("&????????????"), this);
    openImageAct->setShortcuts(QKeySequence::New);
    connect(openImageAct, &QAction::triggered, this, &ImageViewer::on_selectImageFile_clicked);
    fileToolBar->addAction(openImageAct);
//    operateToolBar->addAction(openImageAct);

    const QIcon openFolderIcon = QIcon::fromTheme("document-new", QIcon(":/images/OpenFolder.svg"));
    QAction *openFolderAct = new QAction(openFolderIcon, tr("&????????????"), this);
    openFolderAct->setShortcuts(QKeySequence::New);
    connect(openFolderAct, &QAction::triggered, this, &ImageViewer::on_selectHomeDir_clicked);
    fileToolBar->addAction(openFolderAct);

    fileToolBar->addSeparator();

    const QIcon prevImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Back.svg"));
    QAction *prevImageAct = new QAction(prevImageIcon, tr("&?????????"), this);
    prevImageAct->setShortcuts(QKeySequence::New);
    connect(prevImageAct, &QAction::triggered, this, &ImageViewer::on_readPrevImage_clicked);
    fileToolBar->addAction(prevImageAct);


    const QIcon nextImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Next.svg"));
    QAction *nextImageAct = new QAction(nextImageIcon, tr("&?????????"), this);
    nextImageAct->setShortcuts(QKeySequence::New);
    connect(nextImageAct, &QAction::triggered, this, &ImageViewer::on_readNextImage_clicked);
    fileToolBar->addAction(nextImageAct);

    fileToolBar->addSeparator();

    const QIcon refreshImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Refresh.svg"));
    QAction * refreshImageAct = new QAction(refreshImageIcon, tr("&??????"), this);
    refreshImageAct->setShortcuts(QKeySequence::New);
    connect(refreshImageAct, &QAction::triggered, this, &ImageViewer::on_refreshImage_clicked);
    fileToolBar->addAction(refreshImageAct);

    fileToolBar->addSeparator();

    const QIcon rotateImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Rotate.svg"));
    QAction * rotateImageAct = new QAction(rotateImageIcon, tr("&??????"), this);
    rotateImageAct->setShortcuts(QKeySequence::New);
    connect(rotateImageAct, &QAction::triggered, this, &ImageViewer::on_rotateImage_clicked);
    fileToolBar->addAction(rotateImageAct);

    const QIcon flipImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Flip.svg"));
    QAction * flipImageAct = new QAction(flipImageIcon, tr("&??????"), this);
    flipImageAct->setShortcuts(QKeySequence::New);
    connect(flipImageAct, &QAction::triggered, this, &ImageViewer::on_flipImage_clicked);
    fileToolBar->addAction(flipImageAct);

    fileToolBar->addSeparator();

    const QIcon extendImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/Extend.svg"));
    QAction * extendImageAct = new QAction(extendImageIcon, tr("&????????????"), this);
    extendImageAct->setShortcuts(QKeySequence::New);
    connect(extendImageAct, &QAction::triggered, this, &ImageViewer::on_extendImage_clicked);
    fileToolBar->addAction(extendImageAct);

    const QIcon zoomInImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/ZoomIn.svg"));
    QAction * zoomInImageAct = new QAction(zoomInImageIcon, tr("&??????"), this);
    zoomInImageAct->setShortcuts(QKeySequence::New);
    connect(zoomInImageAct, &QAction::triggered, this, &ImageViewer::on_zoomInImage_clicked);
    fileToolBar->addAction(zoomInImageAct);

    const QIcon zoomOutImageIcon = QIcon::fromTheme("document-new", QIcon(":/images/ZoomOut.svg"));
    QAction * zoomOutImageAct = new QAction(zoomOutImageIcon, tr("&??????"), this);
    zoomOutImageAct->setShortcuts(QKeySequence::New);
    connect(zoomOutImageAct, &QAction::triggered, this, &ImageViewer::on_zoomOutImage_clicked);
    fileToolBar->addAction(zoomOutImageAct);
}

void ImageViewer::initStatusBar(){
    fileIndexLabel = new QLabel();
    filePathLabel = new QLabel();
    fileSizeLabel = new QLabel();
    imageScaleLabel = new QLabel();
    imageSizeLabel = new QLabel();
    fileModDateLabel = new QLabel();

    statusBar()->addWidget(fileIndexLabel,0);
    statusBar()->addWidget(filePathLabel,1);
    statusBar()->addWidget(fileSizeLabel,0);
    statusBar()->addWidget(imageSizeLabel,0);
    statusBar()->addWidget(imageSizeLabel,0);
    statusBar()->addWidget(fileModDateLabel,0);
}


void ImageViewer::readFile(QString filePath){
    if(filePath != ""){

        fileInfo = QFileInfo(filePath);;
        loadFile();
        loadImage("normal");

    }
}

void ImageViewer::readDir(QString dirPath){
    if(dirPath != ""){
        QDir dir(dirPath);
        if(!dir.exists())
            return;

        //????????????????????????.cfg???????????????
        QStringList nameFilters;
        nameFilters<< "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";

        files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
        fileInfo = QFileInfo(dirPath+"/"+files.at(0));
        loadFile();
        loadImage("normal");
    }
}

void ImageViewer::loadFile(){
    current_index = 0;

    QStringList nameFilters;
    nameFilters<< "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
    files = fileInfo.absoluteDir().entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

    for(int i = 0; i< files.size();++i) {
        QString tmp = files.at(i);
        if(tmp == fileInfo.fileName()){
            current_index = i;
        }
    }

    fileIndexLabel->setText(QString::number(current_index + 1)+"/"+QString::number(files.size()));
    filePathLabel->setText(fileInfo.absoluteFilePath());
    fileModDateLabel->setText(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    double fileSize = fileInfo.size();
    QString fileSizeStr;
    if(fileSize / 1024 < 1){
        fileSizeStr = QString::number(fileSize)+" Btyes";
    } else if(fileSize / (1024*1024) < 1){
        fileSize = ( (float)( (int)( (fileSize/1024+0.005)*100 ) ) )/100;
        fileSizeStr = QString::number(fileSize)+" KB";
    } else if(fileSize / (1024*1024*1024) < 1){
        fileSize = ( (float)( (int)( (fileSize/1024/1024+0.005)*100 ) ) )/100;
        fileSizeStr = QString::number(fileSize)+" MB";
    } else {
        fileSizeStr = QString::number(fileSize)+" Btyes";
    }
    fileSizeLabel->setText(fileSizeStr);

    originImage = cv::imread(fileInfo.absoluteFilePath().toLocal8Bit().toStdString()
                             ,cv::IMREAD_UNCHANGED);
}

void ImageViewer::loadImage(QString loadType){
    if(!originImage.empty()){
        imgSize = QSize(originImage.cols, originImage.rows);
        imageSizeLabel->setText(QString::number(originImage.cols)
                                +"x"
                                +QString::number(originImage.rows));
        if(loadType == "normal"){
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "flip"){
            flipImage(originImage).copyTo(originImage);
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "rotate"){
            rotateImage(originImage).copyTo(originImage);
            computeScaleWithView();
            displayImage(resizeImage(originImage));
        } else if(loadType == "zoomIn"){
            zoomInImage();
            displayImage(resizeImage(originImage));
        } else if(loadType == "zoomOut"){
            zoomOutImage();
            displayImage(resizeImage(originImage));
        } else if(loadType == "extend"){
            extendImage();
            displayImage(resizeImage(originImage));
        }

        imageSizeLabel->setText(QString::number(( (float)( (int)( (scaleVar+0.005)*100 ) )))+" %");
    }
}
void ImageViewer::computeScaleWithView(){
    double labelW = ui->scrollArea->size().width()-2;
    double labelH = ui->scrollArea->size().height()-2;
    if(originImage.cols > labelW
            || originImage.rows > labelH){
        double imgW = originImage.cols;
        double imgH = originImage.rows;

        if(imgW > labelW && imgH < labelH){
            scaleVar = imgW/labelW;
        }else if(imgW < labelW && imgH > labelH){
            scaleVar = imgH/labelH;
        }else {
            double scaleVarW = imgW/labelW;
            double scaleVarH = imgH/labelH;
            if(scaleVarW > scaleVarH){
                scaleVar = scaleVarW;
            }else{
                scaleVar = scaleVarH;
            }
        }
        scaleVar = 1 / scaleVar;
    } else {
        scaleVar = 1;
    }
}

void ImageViewer::displayImage(cv::Mat image){
    cv::Mat Rgb;
    QImage Img;
    //    qDebug()<<"image.channels() ="<< image.channels();
    if (image.channels() == 3) {//RGB Img
        cv::cvtColor(image, Rgb, cv::COLOR_BGR2RGB);//??????????????????
        Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
        //        qDebug()<<"Img ="<< Img;
    } else if(image.channels() == 4){
        cv::cvtColor(image, Rgb, cv::COLOR_BGRA2RGBA);//??????????????????
        Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGBA8888);
    } else {//Gray Img
        Img = QImage((const uchar*)(image.data), image.cols, image.rows, image.cols*image.channels(), QImage::Format_Indexed8);
    }
    imgArea->setPixmap(QPixmap::fromImage(Img));
    imgArea->adjustSize();
}
/**************************************
 * ??????????????????
 *************************************/
cv::Mat ImageViewer::resizeImage(cv::Mat orgImage){
    cv::Mat image;
    int width = floor(orgImage.cols * scaleVar);
    int height = floor(orgImage.rows * scaleVar);
    cv::resize(orgImage,image,
               cv::Size(width, height),
               cv::INTER_CUBIC);
    return image;
}

cv::Mat ImageViewer::rotateImage(cv::Mat orgImage){
    cv::Mat dstImage;
    //???????????????
    cv::transpose(orgImage,dstImage);
    cv::flip(dstImage,dstImage,1);
    return dstImage;
}

cv::Mat ImageViewer::flipImage(cv::Mat orgImage){
    cv::Mat dstImage;
    cv::flip(orgImage,dstImage,1);
    return dstImage;
}

void ImageViewer::zoomInImage(){
    if(scaleVar <= 4.9){
        scaleVar = scaleVar+0.1;
    }else{
        scaleVar = 5;
    }
}

void ImageViewer::zoomOutImage(){
    if(scaleVar >= 0.2){
        scaleVar = scaleVar-0.1;
    }else{
        scaleVar = 0.1;
    }
}
void ImageViewer::extendImage(){
    scaleVar = 1;
}
/**************************************
 * ????????????
**************************************/
void ImageViewer::keyPressEvent(QKeyEvent *event){
    if(event->modifiers() == Qt::NoModifier){
        if(event->key() == Qt::Key_Left){
            on_readPrevImage_clicked();
        }else if(event->key() == Qt::Key_Right){
            on_readNextImage_clicked();
        }
    }
}

void ImageViewer::dragEnterEvent(QDragEnterEvent *event){
    if(!event->mimeData()->urls()[0].toLocalFile().right(3).compare("jpg",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(3).compare("png",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(3).compare("bmp",Qt::CaseInsensitive)
            ||!event->mimeData()->urls()[0].toLocalFile().right(4).compare("jpeg",Qt::CaseInsensitive)){
        event->acceptProposedAction();//????????????????????????
    } else {
        event->ignore();//???????????????????????????
    }
}

void ImageViewer::dropEvent(QDropEvent *event){
    readFile(event->mimeData()->urls()[0].toLocalFile());
    creatListWidget();
}

void ImageViewer::on_selectHomeDir_clicked(){
    QString selectDir;
    selectDir = QFileDialog::getExistingDirectory(this,
                                                  "??????????????????",
                                                  "",
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if( selectDir != "" ) {
        readDir(selectDir);
        creatListWidget();
    }
}

void ImageViewer::on_selectImageFile_clicked(){
    QString selectFile;
    QFileInfo fi;
    selectFile = QFileDialog::getOpenFileName(this,
                                              "????????????",
                                              "*.jpg;*.jpeg;*.bmp;*.png");
    if(selectFile != ""){
        readFile(selectFile);
        creatListWidget();
    }
}

void ImageViewer::on_readPrevImage_clicked(){
    if(current_index > 0){
        current_index -= 1;
    }else if(current_index == 0){
        current_index = 0;
    }else{
        return;
    }
    readFile(fileInfo.absolutePath()+"/"+files.at(current_index));
    ui->listWidget->setCurrentRow(current_index);

}

void ImageViewer::on_readNextImage_clicked(){
    if(current_index < files.size() - 1){
        current_index += 1;
    }else if(current_index == files.size() - 1){
        current_index = files.size() - 1;
    }else{
        return;
    }
    readFile(fileInfo.absolutePath()+"/"+files.at(current_index));
    ui->listWidget->setCurrentRow(current_index);
}

void ImageViewer::on_refreshImage_clicked(){
    readFile(fileInfo.absoluteFilePath());
}
void ImageViewer::on_rotateImage_clicked(){
    loadImage("rotate");
}
void ImageViewer::on_flipImage_clicked(){
    loadImage("flip");
}
void ImageViewer::on_zoomInImage_clicked(){
    if(scaleVar < 2){
        loadImage("zoomIn");
    }
}
void ImageViewer::on_zoomOutImage_clicked(){
    if(scaleVar > 0.1){
        loadImage("zoomOut");
    }
}
void ImageViewer::on_extendImage_clicked(){
    if(scaleVar != 1){
        loadImage("extend");
    }
}




