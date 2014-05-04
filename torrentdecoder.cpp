#include "torrentdecoder.h"
#include "ui_torrentdecoder.h"
#include "metainfo.h"

TorrentDecoder::TorrentDecoder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TorrentDecoder),torrentFile(NULL), fileOpened(false),dataRead(false)
{
    ui->setupUi(this);
}

TorrentDecoder::~TorrentDecoder()
{
    delete ui;
}

void TorrentDecoder::on_decodeFileButton_clicked()
{
    if(!fileOpened)
    {
        return;
    }
    if(dataRead)
    {
        return;
    }

    QByteArray arr = torrentFile->readAll();
    MetaInfo mi;
    mi.parse(arr);

    ui->textEdit->append(QString::fromStdString("MAIN TRACKER URL"));
    ui->textEdit->append(QString::fromStdString(mi.getMainTrackerUrl().toStdString()));
    ui->textEdit->append(QString::fromStdString("CREATION DATE: ")+mi.getCreationDate().toString());
    ui->textEdit->append(QString::fromStdString("CREATED BY: ")+mi.getCreatedBy());
    ui->textEdit->append(QString::fromStdString("COMMENT : ")+mi.getComment());

    ui->textEdit->append(QString::fromStdString("\nPIECE LENGTH : "));
    ui->textEdit->append(QString::number(mi.getPieceLength())+QString::fromStdString(" B / ")+
                         (QString::number(mi.getPieceLength()/1024)+QString::fromStdString(" kB")));
    ui->textEdit->append(QString::fromStdString("\nNUMBER OF PIECES: ")+QString::number(mi.getNumberOfPieces()));
    ui->textEdit->append(QString::fromStdString("*********************"));

    if(!mi.isMultiFile())
    {
        ui->textEdit->append(QString::fromStdString("\n THIS TORRENT IS A SINGLE FILE"));
        ui->textEdit->append(QString::fromStdString("*********************"));
        ui->textEdit->append(QString::fromStdString("FILE NAME: ")+mi.getName()+
                             QString::fromStdString(" ' LENGTH: ")+QString::number(mi.getTotalLength()));
    }
    else
    {
        ui->textEdit->append(QString::fromStdString("\n THIS TORRENT IS A MULTI FILE"));
        ui->textEdit->append(QString::fromStdString("*********************"));
        ui->textEdit->append(QString::fromStdString("ROOT DIR NAME: ' ")+mi.getName()+QString::fromStdString(" '"));
        ui->textEdit->append(QString::fromStdString("*********************"));
        ui->textEdit->append(QString::fromStdString("FILES: "));
        foreach (QString path , mi.getMultiFilePaths())
        {
            ui->textEdit->append(QString::fromStdString("PATH: ' ")+path+QString::fromStdString(" ' LENGTH: ")+
                                 QString::number(mi.getFileLengthByPath(path)));
            ui->textEdit->append(QString::fromStdString("**********************"));
        }
    }
    dataRead=true;
}

void TorrentDecoder::on_resetButton_clicked()
{
    ui->textEdit->clear();
    if(torrentFile!=NULL)
    {
        fileOpened=false;
        torrentFile->close();
        delete torrentFile;
    }
}

void TorrentDecoder::on_quitButton_clicked()
{
    qApp->quit();
}

void TorrentDecoder::on_actionOpen_triggered()
{
    if(fileOpened)
    {
        return;
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),QString(), tr("Torrent Files (*.torrent);;Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    if(!fileName.isEmpty())
    {

        torrentFile = new QFile(fileName);
        if(!torrentFile->open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        fileOpened=true;
        dataRead=false;

    }
}
