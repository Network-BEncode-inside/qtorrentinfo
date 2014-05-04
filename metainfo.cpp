#include "bencodeparser.h"
#include "metainfo.h"

#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QStringList>
#include <math.h>

MetaInfo::MetaInfo()
{
    clear();
}

void MetaInfo::clear()
{
    errString = "Unknown error";
    content.clear();
    infoData.clear();
    metaInfoMultiFiles.clear();
    metaInfoAnnounce.clear();
    metaInfoAnnounceList.clear();
    metaInfoCreationDate = QDateTime();
    metaInfoComment.clear();
    metaInfoCreatedBy.clear();
    metaInfoName.clear();
    metaInfoPieceLength = 0;
    metaInfoSha1Sums.clear();
    multiFilesLengths.clear();

}

bool MetaInfo::parse(const QByteArray &data)
{
    clear();
    content = data;

    BencodeParser parser;
    if (!parser.parse(content)) {
        errString = parser.errorString();
        return false;
    }

    infoData = parser.infoSection();

    QMap<QByteArray, QVariant> dict = parser.dictionary();
    if (!dict.contains("info"))
        return false;

    QMap<QByteArray, QVariant> info = qvariant_cast<Dictionary>(dict.value("info"));

    if (info.contains("files")) {
        metaInfoFileForm = MultiFileForm;

        QList<QVariant> files = info.value("files").toList();

        for (int i = 0; i < files.size(); ++i) {
            QMap<QByteArray, QVariant> file = qvariant_cast<Dictionary>(files.at(i));
            QList<QVariant> pathElements = file.value("path").toList();
            QByteArray path;
            foreach (QVariant p, pathElements) {
                if (!path.isEmpty())
                    path += "/";
                path += p.toByteArray();
            }

            MetaInfoMultiFile multiFile;
            multiFile.length = file.value("length").toLongLong();
            multiFile.path = QString::fromUtf8(path);
            multiFile.md5sum = file.value("md5sum").toByteArray();
            metaInfoMultiFiles << multiFile;
            //additional map
            multiFilesLengths.insert(multiFile.path, multiFile.length);
        }

        metaInfoName = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoPieceLength = info.value("piece length").toInt();
        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
            metaInfoSha1Sums << pieces.mid(i, 20);
    }
    else if (info.contains("length"))
    {
        metaInfoFileForm = SingleFileForm;
        metaInfoSingleFile.length = info.value("length").toLongLong();
        metaInfoSingleFile.md5sum = info.value("md5sum").toByteArray();
        metaInfoSingleFile.name = QString::fromUtf8(info.value("name").toByteArray());
        metaInfoSingleFile.pieceLength = info.value("piece length").toInt();

        QByteArray pieces = info.value("pieces").toByteArray();
        for (int i = 0; i < pieces.size(); i += 20)
        {
            metaInfoSingleFile.sha1Sums << pieces.mid(i, 20);
        }

    }

    metaInfoAnnounce = QString::fromUtf8(dict.value("announce").toByteArray());

    if (dict.contains("announce-list")) {
        // ### unimplemented
    }

    if (dict.contains("creation date"))
        metaInfoCreationDate.setTime_t(dict.value("creation date").toInt());
    if (dict.contains("comment"))
        metaInfoComment = QString::fromUtf8(dict.value("comment").toByteArray());
    if (dict.contains("created by"))
        metaInfoCreatedBy = QString::fromUtf8(dict.value("created by").toByteArray());

    return true;
}

//*********MY METHODS USE THIS***************************************

/** Return the main tracker Url*/
QString MetaInfo::getMainTrackerUrl() const
{
    return this->announceUrl();
}

/** Return the filename or root directory name*/
QString MetaInfo::getName() const
{
    if(this->fileForm()==SingleFileForm)
    {
        return this->getSingleFileName();
    }
    else if( this->fileForm()==MultiFileForm)
    {
        return this->name();
    }
    else
    {
        return QString();
    }
}

/** Return total length of a file */
qint64 MetaInfo::getTotalLength() const
{
    return this->totalSize();
}

/** Return length of piece */
int MetaInfo::getPieceLength() const
{
    if(this->fileForm()==SingleFileForm)
    {
        return this->getSingleFilePieceLength();
    }
    else if( this->fileForm()==MultiFileForm)
    {
        return this->pieceLength();
    }
    else
    {
        return -1;
    }
}

/** Return number of pieces */
double MetaInfo::getNumberOfPieces() const
{
    double temp=(double)getTotalLength();
    double chunks = ceil(temp/getPieceLength());
    return chunks;
}

/** Return list of SHA-1 checksums */
QList<QByteArray> MetaInfo::getSha1Sums() const
{
    if(this->fileForm()==SingleFileForm)
    {
        return this->getSingleFileSha1Sums();
    }
    else if( this->fileForm()==MultiFileForm)
    {
        return this->sha1Sums();
    }
    else
    {
        return QList<QByteArray>();
    }
}

/** Return a list of paths of files for multifiles */
QList<QString> MetaInfo::getMultiFilePaths() const
{
    if(this->fileForm()==MultiFileForm)
    {
        QList<QString> list;
        foreach (MetaInfoMultiFile file, multiFiles())
        {
            list.push_back(file.path);
        }
        return list;
    }
    else
    {
        return QList<QString>();
    }
}

/** Return creation date */
QDateTime MetaInfo::getCreationDate() const
{
    return this->creationDate();
}

/** Return comment */
QString MetaInfo::getComment() const
{
    return this->comment();
}

/** Return created by */
QString MetaInfo::getCreatedBy() const
{
    return this->createdBy();
}

/** Return length of the specified file */
qint64 MetaInfo::getFileLengthByPath(QString path) const
{
    if(multiFilesLengths.contains(path))
    {
        return multiFilesLengths.value(path);
    }
    else
    {
        return 0;
    }

}
/** Return true is this file is a multifile */
bool MetaInfo::isMultiFile() const
{
    if(this->fileForm()==MultiFileForm)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//***************END OF MY METHODS***********************************

qint64 MetaInfo::getMetaLength() const
{
    return singleFile().length;
}
QByteArray MetaInfo::getMetaMD5() const
{
    return singleFile().md5sum;
}
QString MetaInfo:: getSingleFileName() const
{
    return singleFile().name;
}
int MetaInfo::getSingleFilePieceLength() const
{
    return singleFile().pieceLength;
}

QList<QByteArray> MetaInfo::getSingleFileSha1Sums() const
{
    return singleFile().sha1Sums;
}


QList<qint64> MetaInfo::getMultiFileLengths() const
{
    QList<qint64> list;
    foreach (MetaInfoMultiFile file, multiFiles())
    {
        list.push_back(file.length);
    }
    return list;
}

QByteArray MetaInfo::infoValue() const
{
    return infoData;
}

QString MetaInfo::errorString() const
{
    return errString;
}

MetaInfo::FileForm MetaInfo::fileForm() const
{
    return metaInfoFileForm;
}

QString MetaInfo::announceUrl() const
{
    return metaInfoAnnounce;
}

QStringList MetaInfo::announceList() const
{
    return metaInfoAnnounceList;
}

QDateTime MetaInfo::creationDate() const
{
    return metaInfoCreationDate;
}

QString MetaInfo::comment() const
{
    return metaInfoComment;
}

QString MetaInfo::createdBy() const
{
    return metaInfoCreatedBy;
}

MetaInfoSingleFile MetaInfo::singleFile() const
{
    return metaInfoSingleFile;
}

QList<MetaInfoMultiFile> MetaInfo::multiFiles() const
{
    return metaInfoMultiFiles;
}

QString MetaInfo::name() const
{
    return metaInfoName;
}

int MetaInfo::pieceLength() const
{
    return metaInfoPieceLength;
}

QList<QByteArray> MetaInfo::sha1Sums() const
{
    return metaInfoSha1Sums;
}

qint64 MetaInfo::totalSize() const
{
    if (fileForm() == SingleFileForm)
        return singleFile().length;

    qint64 size = 0;
    foreach (MetaInfoMultiFile file, multiFiles())
        size += file.length;
    return size;
}
