#ifndef MAIN_H
#define MAIN_H

#include <QxtHttpServerConnector>
#include <QxtHttpSessionManager>
#include <QxtCommandOptions>
#include <QtNetwork>


class Http_response : public QObject
{
    Q_OBJECT
public:
    Http_response(QObject *parent = 0);
    ~Http_response();

public slots:
    void receiveResponse(int http_code);
};


class Push : public QObject
{
    Q_OBJECT

public:
    Push(QString url, QString file_path);
    ~Push();
    void Payload_http();

    QString m_credentials;
    QString m_server;
    QNetworkAccessManager *m_network;


public slots:
    void slotRequestFinished(QNetworkReply *);
    void replyError(QNetworkReply::NetworkError errorCode);


signals:
    void httpResponse(int http_code);



private:
    QFile *m_file;
    QByteArray *m_content;
    QNetworkReply *m_reply;
    QUrl m_url;
    QNetworkRequest m_request;
    QHttpRequestHeader m_header;
    QAuthenticator *m_auth;
    QString m_post_response;
    int m_http_error;
};


#endif // MAIN_H
