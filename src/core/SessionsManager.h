#ifndef OTTER_SESSIONSMANAGER_H
#define OTTER_SESSIONSMANAGER_H

#include <QtCore/QCoreApplication>
#include <QtCore/QPoint>
#include <QtCore/QPointer>

namespace Otter
{

struct WindowHistoryEntry
{
	QString url;
	QString title;
	QPoint position;
	int zoom;

	WindowHistoryEntry() : zoom(100) {}
};

struct WindowHistoryInformation
{
	QList<WindowHistoryEntry> entries;
	int index;

	WindowHistoryInformation() : index(-1) {}
};

struct SessionWindow
{
	QString searchEngine;
	QList<WindowHistoryEntry> history;
	int group;
	int index;
	bool pinned;

	SessionWindow() : group(0), index(-1), pinned(false) {}

	QString url() const
	{
		if (index >= 0 && index < history.count())
		{
			return history.at(index).url;
		}

		return QString();
	}

	QString title() const
	{
		if (index >= 0 && index < history.count())
		{
			return history.at(index).title;
		}

		return QCoreApplication::translate("main", "(Untitled)");
	}

	int zoom() const
	{
		if (index >= 0 && index < history.count())
		{
			return history.at(index).zoom;
		}

		return 100;
	}
};

struct SessionEntry
{
	QList<SessionWindow> windows;
	int index;

	SessionEntry() : index(-1) {}
};

struct SessionInformation
{
	QString path;
	QString title;
	QList<SessionEntry> windows;
	int index;

	SessionInformation() : index(-1) {}
};

class MainWindow;
class WindowsManager;

class SessionsManager : public QObject
{
	Q_OBJECT

public:
	static void createInstance(QObject *parent = NULL);
	static void clearClosedWindows();
	static void registerWindow(WindowsManager *manager);
	static void storeClosedWindow(WindowsManager *manager);
	static void markSessionModified();
	static void setActiveWindow(QWidget *window);
	static SessionsManager* getInstance();
	static QWidget* getActiveWindow();
	static QString getCurrentSession();
	static QString getSessionPath(const QString &path, bool bound = false);
	static QStringList getClosedWindows();
	static SessionInformation getSession(const QString &path);
	static QStringList getSessions();
	static bool restoreClosedWindow(int index = -1);
	static bool restoreSession(const QString &path, MainWindow *window = NULL);
	static bool saveSession(const QString &path = QString(), const QString &title = QString(), MainWindow *window = NULL);
	static bool deleteSession(const QString &path = QString());
	static bool moveSession(const QString &from, const QString &to);
	static bool isLastWindow();
	static bool hasUrl(const QUrl &url, bool activate = false);

protected:
	void timerEvent(QTimerEvent *event);
	void scheduleAutoSave();

private:
	explicit SessionsManager(QObject *parent = NULL);

	int m_autoSaveTimer;

	static SessionsManager *m_instance;
	static QPointer<QWidget> m_activeWindow;
	static QString m_session;
	static QList<WindowsManager*> m_managers;
	static QList<SessionEntry> m_closedWindows;
	static bool m_dirty;

signals:
	void closedWindowsChanged();
};

}

#endif
