#include <QtCore/QCoreApplication>
#include "main.h"
#include <iostream>



void replyError(QNetworkReply::NetworkError errorCode)
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
    QNetworkRequest m_request;
    QHttpRequestHeader m_header;

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


    QObject::connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));


    url.setUrl("http://localhost/payload/create/");
    //url.setUrl(m_server + "/host/update/");
    m_request.setUrl(url);
    m_reply = m_network->post(m_request, "json.toUtf8()");



    return a.exec();
}





