#include <iostream>
#include <stdexcept>
#include "Demos.h"
#include "TestData.h"

int main()
{
    try
    {
        prepareTestData();

        runRawSqliteDemo();
        runRepositoryDemo();
        runQueryDemo();
        runAggregateDemo();
        runJoinDemo();
        runTransactionDemo();
        runRollbackDemo();
        runWithoutTransactionDemo();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Application error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
