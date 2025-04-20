#pragma once

#include "lib/enum/clienttype.hpp"
#include "lib/settings.hpp"
#include "lib/logmessage.hpp"

#include "spotifyclient/helper.hpp"

#include <QDateTime>
#include <QFileInfo>
#include <QInputDialog>
#include <QList>
#include <QProcess>
#include <QRegularExpression>
#include <QStringList>

namespace SpotifyClient
{
	/**
	 * Manages any running spotify client instance
	 */
	class Runner: public QObject
	{
	Q_OBJECT

	public:
		Runner(const lib::settings &settings, const lib::paths &paths, QWidget *parent);
		~Runner() override;

		void start();

		static auto getLog() -> const std::vector<lib::log_message> &;
		auto isRunning() const -> bool;

	signals:
		/**
		 * Client status changed, usually after attempted start
		 * @param status Error message, or empty string on success
		 */
		void statusChanged(const QString &status);

	private:
		QProcess *process;
		QWidget *parentWidget = nullptr;
		QString path;
		static std::vector<lib::log_message> log;
		const lib::settings &settings;
		const lib::paths &paths;
		lib::client_type clientType;

		void logOutput(const QByteArray &output, lib::log_type logType);
		static auto joinArgs(const QStringList &args) -> QString;

		auto getCachePath() const -> ghc::filesystem::path;
		auto isLoggedIn() const -> bool;
		auto resetCredentials() const -> bool;

		void onReadyReadOutput();
		void onReadyReadError();
		void onStarted();
		void onErrorOccurred(QProcess::ProcessError error);
	};
}
