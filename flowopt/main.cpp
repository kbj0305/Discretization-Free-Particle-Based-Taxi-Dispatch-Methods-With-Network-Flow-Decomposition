#define ILO_WINDOWS
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
int T = 25;
short dur[32001][15001],cost[32001][15001];
int main(int argc, char** argv)
{
    double *h;
	IloEnv env;

	int d = 10, c = 10;
	FILE* in = fopen("dur.txt", "r");
	FILE* out = fopen("move.txt", "w");
	fscanf(in, "%d %d", &d, &c);
	for (int i = 0; i < d; i++)
		for (int j = 0; j < c; j++)
        {
			fscanf(in, "%d", dur[i] + j);
            assert(dur[i][j]>0);
            assert(dur[i][j]<25);
        }
	for (int i = 0; i < d; i++)
		for (int j = 0; j < c; j++)
			cost[i][j] = 1;

	try {
		IloModel model(env);
		IloNumVarArray* n = new IloNumVarArray[c];
		IloNumVarArray* x = new IloNumVarArray[c];
		IloNumVarArray* m = new IloNumVarArray[c];
		IloExpr obj(env);
		for (int i = 0; i < c; i++)
		{
			n[i] = IloNumVarArray(env);
			x[i] = IloNumVarArray(env);
			for (int j = 0; j < T; j++)
			{
				n[i].add(IloNumVar(env));
				x[i].add(IloNumVar(env, 0, 1));
			}
			m[i] = IloNumVarArray(env);
			for (int j = 0; j < d; j++)
				m[i].add(IloNumVar(env, 0, 1));
		}
		for (int i = 0; i < d; i++)
		{
			IloExpr expr(env);
			for (int j = 0; j < c; j++)
				expr += m[j][i];
			model.add(expr <= 1);
		}
		for (int i = 0; i < c; i++)
		{
			IloExprArray expr(env);
			for (int j = 0; j < T; j++)
				expr.add(IloExpr(env));
			for (int j = 0; j < d; j++)
			{
				expr[dur[j][i] - 1] += m[i][j];
				//obj -= m[i][j] * cost[j][i];
			}
			for (int j = 1; j < T; j++)
				expr[j] += n[i][j - 1] - x[i][j - 1];
			for (int j = 0; j < T; j++)
			{
				model.add(n[i][j] == expr[j]);
				model.add(n[i][j] >= x[i][j]);
				obj += x[i][j]*(100000-100*j);
			}
		}

		model.add(IloMaximize(env, obj));
		IloCplex cplex(model);
		//cplex.setParam((IloCplex::NumParam)CPX_PARAM_EPPER,0.1);
		if (!cplex.solve()) {
			env.error() << "Failed to optimize LP." << endl;
			throw(-1);
		}
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value = " << cplex.getObjValue() << endl;
		int a = cplex.getObjValue();
		printf("%d\n", a);
		for (int i = 0; i < c; i++)
			for (int j=0; j < d; j++)
			{
				if (cplex.getValue(m[i][j]) > 0)
					fprintf(out, "%d %d\n", j, i);
			}
	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}
		env.end();
	return 0;
}
