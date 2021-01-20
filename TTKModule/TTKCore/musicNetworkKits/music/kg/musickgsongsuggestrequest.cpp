#include "musickgsongsuggestrequest.h"
#include "musickgqueryinterface.h"

MusicKGSongSuggestRequest::MusicKGSongSuggestRequest(QObject *parent)
    : MusicSongSuggestRequest(parent)
{

}

void MusicKGSongSuggestRequest::startToSearch(const QString &text)
{
    if(!m_manager)
    {
        return;
    }

    TTK_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(text));
    deleteAll();

    m_interrupt = true;

    QNetworkRequest request;
    request.setUrl(MusicUtils::Algorithm::mdII(KG_SUGGEST_URL, false).arg(text));
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(KG_UA_URL, ALG_UA_KEY, false).toUtf8());
    MusicObject::setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicKGSongSuggestRequest::downLoadFinished()
{
    if(!m_reply || !m_manager)
    {
        deleteAll();
        return;
    }

    TTK_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_items.clear();
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QJson::Parser parser;
        bool ok;
        const QVariant &data = parser.parse(m_reply->readAll(), &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value["error_code"].toInt() == 0 && value.contains("data"))
            {
                const QVariantList &datas = value["data"].toList();
                for(const QVariant &var : qAsConst(datas))
                {
                    if(m_interrupt) return;

                    if(var.isNull())
                    {
                        continue;
                    }

                    value = var.toMap();
                    if(value["LableName"].toString().isEmpty())
                    {
                        for(const QVariant &var : value["RecordDatas"].toList())
                        {
                            if(m_interrupt) return;

                            if(var.isNull())
                            {
                                continue;
                            }

                            value = var.toMap();
                            MusicResultsItem item;
                            item.m_name = value["HintInfo"].toString();
                            m_items << item;
                        }
                    }
                    break;
                }
            }
        }
    }

    Q_EMIT downLoadDataChanged(QString());
    deleteAll();
}
