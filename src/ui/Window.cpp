#include "Window.h"
#include "../backends/web/WebBackendsManager.h"

#include "ui_Window.h"

namespace Otter
{

Window::Window(WebWidget *widget, QWidget *parent) : QWidget(parent),
	m_webWidget(widget),
	m_isPinned(false),
	m_ui(new Ui::Window)
{
	if (m_webWidget)
	{
		m_webWidget->setParent(this);
	}
	else
	{
		m_webWidget = WebBackendsManager::getBackend()->createWidget(this);
	}

	m_ui->setupUi(this);
	m_ui->backButton->setDefaultAction(getAction(GoBackAction));
	m_ui->forwardButton->setDefaultAction(getAction(GoForwardAction));
	m_ui->reloadButton->setDefaultAction(getAction(ReloadAction));
	m_ui->verticalLayout->addWidget(m_webWidget);
	m_ui->lineEdit->setFocus();

	connect(m_ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(loadUrl()));
	connect(m_webWidget, SIGNAL(statusMessageChanged(QString,int)), this, SIGNAL(statusMessageChanged(QString,int)));
	connect(m_webWidget, SIGNAL(titleChanged(QString)), this, SIGNAL(titleChanged(QString)));
	connect(m_webWidget, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)));
	connect(m_webWidget, SIGNAL(urlChanged(QUrl)), this, SLOT(updateUrl(QUrl)));
	connect(m_webWidget, SIGNAL(iconChanged(QIcon)), this, SIGNAL(iconChanged(QIcon)));
	connect(m_webWidget, SIGNAL(loadingChanged(bool)), this, SIGNAL(loadingChanged(bool)));
	connect(m_webWidget, SIGNAL(zoomChanged(int)), this, SIGNAL(zoomChanged(int)));
	connect(m_webWidget, SIGNAL(isPrivateChanged(bool)), this, SIGNAL(isPrivateChanged(bool)));
}

Window::~Window()
{
	delete m_ui;
}

void Window::print(QPrinter *printer)
{
	m_webWidget->print(printer);
}

Window* Window::clone(QWidget *parent)
{
	if (!isClonable())
	{
		return NULL;
	}

	Window *window = new Window(m_webWidget->clone(), parent);
	window->setPinned(isPinned());

	return window;
}

QAction *Window::getAction(WebAction action)
{
	return m_webWidget->getAction(action);
}

void Window::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch (event->type())
	{
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);

			break;
		default:
			break;
	}
}

void Window::setZoom(int zoom)
{
	m_webWidget->setZoom(zoom);
}

void Window::setUrl(const QUrl &url)
{
	m_webWidget->setUrl(url);
}

void Window::setPinned(bool pinned)
{
	if (pinned != m_isPinned)
	{
		m_isPinned = pinned;

		emit isPinnedChanged(pinned);
	}
}

void Window::setPrivate(bool enabled)
{
	m_webWidget->setPrivate(enabled);
}

void Window::loadUrl()
{
	setUrl(QUrl(m_ui->lineEdit->text()));
}

void Window::updateUrl(const QUrl &url)
{
	m_ui->lineEdit->setText(url.toString());
}

QUndoStack *Window::getUndoStack()
{
	return m_webWidget->getUndoStack();
}

QString Window::getTitle() const
{
	return m_webWidget->getTitle();
}

QUrl Window::getUrl() const
{
	return m_webWidget->getUrl();
}

QIcon Window::getIcon() const
{
	return m_webWidget->getIcon();
}

int Window::getZoom() const
{
	return m_webWidget->getZoom();
}

bool Window::isClonable() const
{
	return true;
}

bool Window::isEmpty() const
{
	const QUrl url = m_webWidget->getUrl();

	return (url.scheme() == "about" && (url.path().isEmpty() || url.path() == "blank"));
}

bool Window::isLoading() const
{
	return m_webWidget->isLoading();
}

bool Window::isPinned() const
{
	return m_isPinned;
}

bool Window::isPrivate() const
{
	return m_webWidget->isPrivate();
}

void Window::triggerAction(WebAction action, bool checked)
{
	m_webWidget->triggerAction(action, checked);
}

}
