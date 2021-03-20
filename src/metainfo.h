#ifndef METAINFO_H
 #define METAINFO_H

 #include <QByteArray>
 #include <QDateTime>
 #include <QList>
 #include <QMap>
 #include <QString>
 #include <QStringList>
 #include <QVariant>

 struct MetaInfoSingleFile
 {
     qint64 length;
     QByteArray md5sum;
     QString name;
     int pieceLength;
     QList<QByteArray> sha1Sums;
 };

 struct MetaInfoMultiFile
 {
     qint64 length;
     QByteArray md5sum;
     QString path;
 };

 class MetaInfo
 {

 public:
     enum FileForm {
         SingleFileForm,
         MultiFileForm
     };

     MetaInfo();
     void clear();

     bool parse(const QByteArray &data);
     QString errorString() const;

     QByteArray infoValue() const;

     FileForm fileForm() const;
     QString announceUrl() const;
     QStringList announceList() const;
     QDateTime creationDate() const;
     QString comment() const;
     QString createdBy() const;

     // For single file form
     MetaInfoSingleFile singleFile() const;

     // For multifile form
     QList<MetaInfoMultiFile> multiFiles() const;
     QString name() const;
     int pieceLength() const;
     QList<QByteArray> sha1Sums() const;

     // Total size
     qint64 totalSize() const;

     //*********** MY METHODS  USE THIS**************
     QString getMainTrackerUrl() const;
     QString getName() const;
     qint64 getTotalLength() const;
     int getPieceLength() const;
     double getNumberOfPieces() const;
     QList<QByteArray> getSha1Sums() const;
     QList<QString> getMultiFilePaths() const;
     QList<qint64> getMultiFileLengths() const;
     QDateTime getCreationDate() const;
     QString getComment() const;
     QString getCreatedBy() const;
     qint64 getFileLengthByPath(QString path) const;
     int getSingleFilePieceLength() const;
     QMap<QString,qint64> getMultiFileLengthMap() const;
     bool isMultiFile() const;
     //**********END OF MY METHODS********************

     // OTHER UNUSED METHODS
     qint64 getMetaLength() const;
     QByteArray getMetaMD5() const;
     QString getSingleFileName() const;
     QList<QByteArray> getSingleFileSha1Sums() const;

private:
     QString errString;
     QByteArray content;
     QByteArray infoData;

     FileForm metaInfoFileForm;
     MetaInfoSingleFile metaInfoSingleFile;
     QList<MetaInfoMultiFile> metaInfoMultiFiles;
     QString metaInfoAnnounce;
     QStringList metaInfoAnnounceList;
     QDateTime metaInfoCreationDate;
     QString metaInfoComment;
     QString metaInfoCreatedBy;
     QString metaInfoName;
     int metaInfoPieceLength;
     QList<QByteArray> metaInfoSha1Sums;
     // my fields
     QMap<QString, qint64> multiFilesLengths;
 };

 #endif
