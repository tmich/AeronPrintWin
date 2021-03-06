#pragma once
#include "sqlite3.h"

namespace sqlite
{
	/* classe per gestire l'accesso concorrente al Database */
	/*static std::mutex mutex_;

	class LockDB
	{
	public:
		LockDB() {}

		void Acquire() {
			mutex_.lock();
			OutputDebugString(L"lock acquisito\n");
		}

		void Release() {
			mutex_.unlock();
			OutputDebugString(L"lock rilasciato\n");
		}

		~LockDB() { }
	};*/

	/* Eccezione SQLite */
	class SqliteException : public std::exception
	{
	public:
		SqliteException(std::string message) : message_(message) {};

		virtual const char * what() const override
		{
			return message_.c_str();
		}
	protected:
		std::string message_;
	};

	/* Classe che incapsula un cursore restituito da una query */
	class Cursor
	{
	public:
		Cursor();
		Cursor(sqlite3 * db, std::string query);
		Cursor(sqlite3_stmt * stmt);
		~Cursor();

		bool Next();

		int GetInt(int columnIndex);
		std::string GetText(int columnIndex);
		double GetDouble(int columnIndex);
	private:
		sqlite3_stmt * stmt_;
		sqlite3 * db_;
		//std::mutex mutex_;
	};

	/* Classe che incapsula una transaction. */
	class Transaction
	{
	public:
		Transaction(sqlite3 * db);
		~Transaction();

		void Commit();
		void Rollback();

		/* Esegue il comando (INSERT, UPDATE, DELETE) e torna il numero di righe modificate. */
		int Execute(std::string command);

		/* Ritorna l'ultimo rowid creato */
		long long int GetLastInsertRowid();
	protected:
		bool pending;
		sqlite3 * db_;
	};

	/* Classe che incapsula una connessione al db (RAII) */
	class Connection
	{
	public:
		static std::string DATABASE_PATH;

		Connection();
		~Connection();

		void Open(std::string databaseName);
		void Close();

		operator sqlite3 * () const { return db_; }

		/* Esegue il comando (INSERT, UPDATE, DELETE) e torna il numero di righe modificate. */
		int Execute(std::string command);

		/* Ritorna l'ultimo rowid creato */
		long long int GetLastInsertRowid();

		//Cursor ExecuteQuery(std::string query);

		Transaction BeginTransaction();
	protected:
		sqlite3 * db_{ nullptr };
		//std::mutex mutex;
	};
}