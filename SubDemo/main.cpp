// #include "opencv2/imgcodecs.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>
#include <QUrl>

class PublicIPFetcher : public QObject
{
    Q_OBJECT
  private:
    QNetworkAccessManager* manager;
    QStringList ipv4Services = {"https://api.ipify.org", "https://icanhazip.com", "https://ipinfo.io/ip", "https://checkip.amazonaws.com"};

  public:
    PublicIPFetcher(QObject* parent = nullptr)
        : QObject(parent)
    {
        manager = new QNetworkAccessManager(this);
    }

    void fetchPublicIP()
    {
        // 尝试第一个服务
        fetchFromService(0);
    }

  private:
    void fetchFromService(int index)
    {
        if (index >= ipv4Services.size()) {
            qDebug() << "所有服务都失败";
            Q_EMIT fetchFailed();
            return;
        }

        QNetworkRequest request((QUrl(ipv4Services[index])));
        request.setRawHeader("User-Agent", "Mozilla/5.0");
        QNetworkReply* reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, [=, this]() {
            if (reply->error() == QNetworkReply::NoError) {
                QString ip = QString(reply->readAll()).trimmed();
                qDebug() << "公网IP:" << ip << "(来源:" << ipv4Services[index] << ")";
                Q_EMIT ipReceived(ip);
            } else {
                qDebug() << "服务" << index << "失败，尝试下一个";
                fetchFromService(index + 1);  // 尝试下一个服务
            }
            reply->deleteLater();
        });
    }

  Q_SIGNALS:
    void ipReceived(const QString& ip);
    void fetchFailed();
};

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager* manager = new QNetworkAccessManager(&app);
    QTimer* timer = new QTimer(&app);

    // 定义请求函数
    auto makeRequest = [manager]() {
        PublicIPFetcher* checker = new PublicIPFetcher();

        QObject::connect(checker, &::PublicIPFetcher::ipReceived, [checker](const QString& ip) {
            qDebug() << "===== 对方服务器看到的 IPv4 地址:" << ip << "=====";
            if (checker) {
                checker->deleteLater();
            }
        });

        QObject::connect(checker, &::PublicIPFetcher::ipReceived, [checker](const QString& ip) {
            qDebug() << "===== 对方服务器看到的 IPv6 地址:" << ip << "=====";
            if (checker) {
                checker->deleteLater();
            }
        });
        checker->fetchPublicIP();

        QNetworkRequest request(QUrl("https://www.baidu.com/"));
        request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");

        QNetworkReply* reply = manager->get(request);

        QObject::connect(reply, &QNetworkReply::finished, [reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                qDebug() << "Status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                qDebug() << "Content length:" << reply->readAll().length();
            } else {
                qDebug() << "Error:" << reply->errorString();
            }
            reply->deleteLater();
        });
    };

    // 立即执行第一次请求
    makeRequest();

    // 设置定时器，每300ms执行一次
    QObject::connect(timer, &QTimer::timeout, makeRequest);
    timer->start(3000);

    return app.exec();
}

#include "main.moc"  // 必须在文件末尾添加这一行
