#include <QtCore/QCoreApplication>
#include "main.h"
#include <iostream>



Http_response::Http_response(QObject* parent) : QObject(parent)
{}

Http_response::~Http_response()
{}

void Http_response::receiveResponse(int http_code)
{
    qDebug() << "HTTP CODE : " << http_code;
}



Push::Push(QString url, QString file_path)
{
    //m_auth->setUser("aaa");
    m_network = new QNetworkAccessManager(this);
    m_post_response = "";
    m_http_error = 0;
    m_request.setRawHeader("content-type", "application/xml");



/*
    QFile file("in.txt");
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

         while (!file.atEnd()) {
             QByteArray line = file.readLine();
             process_line(line);

*/

    this->connect( m_network, SIGNAL(finished(QNetworkReply *)),SLOT(slotRequestFinished(QNetworkReply *)));

/*
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
                            SLOT(slotSetProgress(qint64,qint64)));
*/

}


Push::~Push()
{}



void Push::Payload_http() {

    qDebug() << "payload and pass : " << m_credentials;


    QDateTime timestamp = QDateTime::currentDateTime();
    QString s_timestamp = "geekast_" + timestamp.toString("hhmmssz-ddMMyyyy")  + ".json";



    m_request.setRawHeader("Authorization", "Basic " + QByteArray((m_credentials).toAscii()).toBase64());
    m_request.setRawHeader("content-type", "multipart/form-data");
    m_request.setRawHeader("Host", url.encodedHost());

    m_request.setRawHeader("User-Agent", "geekast");
    m_request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    m_request.setRawHeader("Accept-Language", "en,en-us;q=0.7,en;q=0.3");
    m_request.setRawHeader("Accept-Encoding", "gzip,deflate");
    m_request.setRawHeader("Accept-Charset", "windows-1251,utf-8;q=0.7,*;q=0.7");
    m_request.setRawHeader("Keep-Alive", "300");
    m_request.setRawHeader("Connection", "keep-alive");
    m_request.setRawHeader("X-payloadfilename", s_timestamp.toUtf8());
    //request.setRawHeader("Referer", QString("http://%1data.cod.ru/").arg(region).toAscii());
    //m_request.setRawHeader("Content-Type", QByteArray("multipart/form-data; boundary=").append(boundaryRegular));

    // qDebug() << QString::fromAscii(QByteArray(m_request.rawHeader( "Authorization" )).fromBase64());

    qDebug() << m_request.rawHeader( "Authorization" ) << m_credentials.toAscii().toBase64();



    // UPDATE
    url.setUrl("http://" + m_server + "/payload/create/");
    //url.setUrl(m_server + "/host/update/");
    qDebug() << "PAYLOAD UPDATE" << " SERVER : " << m_server << "pass : " << m_credentials;
    m_request.setUrl(url);
    m_reply = m_network->post(m_request, "json.toUtf8()");


    this->connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
}




void Push::slotRequestFinished(QNetworkReply *) {

    m_http_error = m_reply->error();
    qDebug() << "HTTP ERROR : " << m_http_error;

    emit httpResponse(m_http_error);

    m_post_response = m_reply->readAll();

    qDebug() << "HTTP RESPONSE : " << m_post_response;

    exit(0);
}


void Push::replyError(QNetworkReply::NetworkError errorCode)
{

    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    switch(errorCode)
    {
    case QNetworkReply::TimeoutError:
    {
        qDebug() << "Push::replyError HTTP ERROR TIMEOUT !";
        //m_requestList.remove(QString::fromStdString(m_request));
        reply->deleteLater();
        //finished();
    }
    case QNetworkReply::HostNotFoundError:
    {
        qDebug() << "Push::replyError HOST NOT FOUND";
        //m_requestList.remove(QString::fromStdString(m_request));
        reply->deleteLater();
        //finished();
    }
    case QNetworkReply::ConnectionRefusedError:
    {
        qDebug() << "Push::replyError Connection refused";
        //m_requestList.remove(QString::fromStdString(m_request));
        reply->deleteLater();
        //finished();
    }

    default:
    {
        qDebug() << "Push::replyError HTTP ERROR DEFAULT ! errorcode : " << errorCode;
        break;
    }
    }
}


int main(int argc, char *argv[])
{
    bool verbose;
    QString param_url;
    QString param_file;
    QUrl url;

    QCoreApplication a(argc, argv);

    QxtHttpServerConnector m_connector;
    QxtHttpSessionManager m_session;


    QxtCommandOptions options;

    options.add("debug", "show debug informations");
    options.alias("debug", "d");
    options.add("url", "set the url", QxtCommandOptions::Required);
    options.alias("url", "url");
    options.add("file", "set the absolute file path", QxtCommandOptions::Required);
    options.alias("file", "file");

    options.add("verbose", "show more information about the process; specify twice for more detail", QxtCommandOptions::AllowMultiple);
    options.alias("verbose", "v");
    options.add("help", "show this help text");
    options.alias("help", "h");
    options.parse(QCoreApplication::arguments());
    if(options.count("help") || options.showUnrecognizedWarning()) {
        options.showUsage();
        return -1;
    }

    verbose = options.count("verbose");
    if(options.count("url")) {
        param_url = options.value("url").toString();
    }
    else {
        std::cout << "cli_post_http: --url requires a parameter" << std::endl;
        options.showUsage();
        return -1;
    }



    if(options.count("file")) {
        param_file = options.value("file").toString();
    }
    else {
        std::cout << "cli_post_http: --file requires a parameter" << std::endl;
        options.showUsage();
        return -1;
    }


    Push l_push;
    Http_response l_http_response;

    l_push.m_server = param_url;
    l_push.m_credentials = "test@gmail.com";


    QObject::connect(&l_push, SIGNAL(httpResponse(int)), &l_http_response, SLOT(receiveResponse(int)));


    l_push.Payload_http();

    return a.exec();
}





