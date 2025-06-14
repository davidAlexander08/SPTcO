#include "C_ModeloOtimizacao.h"

#include "C_EntradaSaidaDados.h"

#include "C_ProcessoEstocastico.h"

#include <time.h>

#include <stdio.h>

#include "mpi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModeloOtimizacao::ModeloOtimizacao(const IdModeloOtimizacao a_idModeloOtimizacao, Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try { 
		   
		lista_variaveis_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());
		lista_equacoes_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());
		lista_inequacoes_instanciadas = std::vector<std::vector<std::string>>(TipoSubproblemaSolver_Excedente, std::vector<std::string>());

		contadorLog10005 = 0;
		escreverLog10005(TipoSubproblemaSolver_Nenhum, IdIteracao_Nenhum, IdEstagio_Nenhum, IdCenario_Nenhum, IdRealizacao_Nenhum, std::string());

		INICIALIZA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		setAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, a_idModeloOtimizacao);
		std::cout << "CRIANDO MODELO OTIMIZACAO" << "\n";
		criarModeloOtimizacao(a_dados, a_entradaSaidaDados);
		std::cout << "CRIOU MODELO OTIMIZACAO" << "\n";
		a_dados.esvaziar();

	}
	catch (const std::exception&erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::ModeloOtimizacao(" + getString(a_idModeloOtimizacao) + ",a_dados,a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // ModeloOtimizacao::ModeloOtimizacao(const IdModeloOtimizacao a_idModeloOtimizacao, Dados &a_dados) {

ModeloOtimizacao::ModeloOtimizacao(const ModeloOtimizacao & instanciaCopiar){
	try {
		
		COPIA_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

		throw std::invalid_argument("Uma instancia de ModeloOtimizacao nao eh passivel de copia.");
	
	}
	catch (const std::exception&erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::ModeloOtimizacao(const ModeloOtimizacao &instanciaCopiar): \n" + std::string(erro.what())); }

}

void ModeloOtimizacao::esvaziar() {
	try {

		EXCLUI_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)

	}
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::esvaziar(): \n" + std::string(erro.what())); }

}
//////////////////////////////////////////////////////////////////////

ModeloOtimizacao::~ModeloOtimizacao() {

	try {
		EXCLUI_SMART_ELEMENTO(ModeloOtimizacao, SMART_ELEMENTO_MODELO_OTIMIZACAO)
	} // try
	catch (const std::exception& erro) { std::cout << "ModeloOtimizacao::~ModeloOtimizacao(): \n" << erro.what() << std::endl; }

} // ModeloOtimizacao::~ModeloOtimizacao() {


//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdVariavelEstado, double> &a_estado){

	try{ 

		SmartEnupla<IdCorteBenders, IdCorteBenders> mapeamento_cortes_sequenciados = vetorEstagio.att(a_idEstagio).sequenciarCorteBenders();

		if (mapeamento_cortes_sequenciados.size() > 0) {

			const IdEstagio estagio_anterior = IdEstagio(a_idEstagio - 1);

			for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

				if (idx_ine_CB_ZF_4.at(tss).size() > 0) {
					if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).size() > 0) {

						for (Periodo periodo = idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorInicial(); periodo <= idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorFinal(); idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).incrementarIterador(periodo)) {
							if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).size() > 0) {
								for (IdRealizacao idReal = idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorInicial(); idReal <= idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorFinal(); idReal++) {

									if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).size() > 0) {

										SmartEnupla<IdCorteBenders, std::vector<int>> idx_ine_CB_ZF_4_novo(IdCorteBenders_1, std::vector<std::vector<int>>(mapeamento_cortes_sequenciados.getIteradorFinal(), std::vector<int>()));

										for (IdCorteBenders idCorte = idx_ine_CB_ZF_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZF_4_novo.getIteradorFinal(); idCorte++) {

											const IdCorteBenders idCorte_old = mapeamento_cortes_sequenciados.at(idCorte);

											if ((idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorInicial() <= idCorte_old) && (idCorte_old <= idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorFinal())) {
												if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
													idx_ine_CB_ZF_4_novo.at(idCorte) = idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old);
													vetorEstagio.att(estagio_anterior).getSolver(tss)->setNomeRestricao(idx_ine_CB_ZF_4_novo.at(idCorte).at(0), getNomeIneLinear_CB_ZF(tss, estagio_anterior, periodo, idReal, idCorte));
												} // if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
											}
										} // for (IdCorteBenders idCorte = idx_ine_CB_ZF_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZF_4_novo.getIteradorFinal(); idCorte++) {

										idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal) = idx_ine_CB_ZF_4_novo;

									} // if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).size() > 0) {
								} // for (IdRealizacao idReal = idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorInicial(); idReal <= idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo).getIteradorFinal(); idReal++) {
							} // if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).at(periodo) > 0) {
						} // for (Periodo periodo = idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorInicial(); periodo <= idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).getIteradorFinal(); idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).incrementarIterador(periodo)) {
					} // if (idx_ine_CB_ZF_4.at(tss).at(estagio_anterior).size() > 0){
				} // if (idx_ine_CB_ZF_4.at(tss).size() > 0) {

				if (idx_ine_CB_ZT_4.at(tss).size() > 0) {
					if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).size() > 0) {

						for (Periodo periodo = idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorInicial(); periodo <= idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorFinal(); idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).incrementarIterador(periodo)) {
							if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).size() > 0) {
								for (IdRealizacao idReal = idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorInicial(); idReal <= idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorFinal(); idReal++) {

									if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).size() > 0) {

										SmartEnupla<IdCorteBenders, std::vector<int>> idx_ine_CB_ZT_4_novo(IdCorteBenders_1, std::vector<std::vector<int>>(mapeamento_cortes_sequenciados.getIteradorFinal(), std::vector<int>()));

										for (IdCorteBenders idCorte = idx_ine_CB_ZT_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZT_4_novo.getIteradorFinal(); idCorte++) {

											const IdCorteBenders idCorte_old = mapeamento_cortes_sequenciados.at(idCorte);

											if ((idx_ine_CB_ZT_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorInicial() <= idCorte_old) && (idCorte_old <= idx_ine_CB_ZT_4.at(tss).at(estagio_anterior).at(periodo).at(idReal).getIteradorFinal())) {
												if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
													idx_ine_CB_ZT_4_novo.at(idCorte) = idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old);
													vetorEstagio.att(a_idEstagio).getSolver(tss)->setNomeRestricao(idx_ine_CB_ZT_4_novo.at(idCorte).at(0), getNomeIneLinear_CB_ZT(tss, a_idEstagio, periodo, idReal, idCorte));
												} // if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).at(idCorte_old).size() > 0) {
											}
										} // for (IdCorteBenders idCorte = idx_ine_CB_ZT_4_novo.getIteradorInicial(); idCorte <= idx_ine_CB_ZT_4_novo.getIteradorFinal(); idCorte++) {

										idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal) = idx_ine_CB_ZT_4_novo;

									} // if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).at(idReal).size() > 0) {
								} // for (IdRealizacao idReal = idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorInicial(); idReal <= idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo).getIteradorFinal(); idReal++) {
							} // if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).at(periodo) > 0) {
						} // for (Periodo periodo = idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorInicial(); periodo <= idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).getIteradorFinal(); idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).incrementarIterador(periodo)) {
					} // if (idx_ine_CB_ZT_4.at(tss).at(a_idEstagio).size() > 0){
				} // if (idx_ine_CB_ZT_4.at(tss).size() > 0) {

			} // for (TipoSubproblemaSolver tss = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tss < TipoSubproblemaSolver_Excedente; tss++) {

		} // if (mapeamento_cortes_sequenciados.size() > 0) {

		vetorEstagio.att(a_idEstagio).instanciarCorteBenders(a_rhs, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){

void ModeloOtimizacao::instanciarCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double>& a_rhs, const SmartEnupla<IdRealizacao, double>& a_rhs_primal, const SmartEnupla<IdRealizacao, double>& a_rhs_estado, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>>& a_coeficiente, const SmartEnupla<IdVariavelEstado, double>& a_estado) {

	try {

		vetorEstagio.att(a_idEstagio).instanciarCorteBenders(a_rhs, a_rhs_primal, a_rhs_estado, a_coeficiente, a_estado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciaCorteBenders(" + getFullString(a_idEstagio) + ",a_rhs,a_rhs_primal,a_rhs_estado,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addCorteBenders(const IdEstagio a_idEstagio, const SmartEnupla<IdRealizacao, double> &a_rhs, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_coeficiente, const SmartEnupla<IdRealizacao, SmartEnupla<IdVariavelEstado, double>> &a_estado){


void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try{

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//Cortes_dominados");

		bool primeiro_corte = false;
		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

			if (vetorEstagio.att(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

				if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, bool())) {

					if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

						a_entradaSaidaDados.setAppendArquivo(primeiro_corte);

						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_primal.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs_primal);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_rhs_estado.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_rhs_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_idEstagio) + "_corteBenders_estado.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttVetorCorteBenders_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_idEstagio) + "_corteBenders_coeficientes.csv", idCorteBenders, vetorEstagio.att(a_idEstagio), AttMatrizCorteBenders_coeficiente);

					} // if ((a_idProcesso == IdProcesso_mestre) && (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool()))) {

					vetorEstagio.att(a_idEstagio).vetorCorteBenders.rem(idCorteBenders);

					if (!primeiro_corte)
						primeiro_corte = true;

				} // if (getAtributo(a_idEstagio, idCorteBenders, AttComumCorteBenders_is_corte_dominado, bool())) {

			} // if (vetorEstagio.att(a_idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {

		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_idEstagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCortesBendersDominados(" + getFullString(a_idEstagio) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCortesBendersDominados(const IdEstagio a_idEstagio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados){

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		std::vector<TipoSubproblemaSolver> lista_subproblemaSolver; 

		if (a_idEstagio > getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()))
			lista_subproblemaSolver = vetorEstagio.att(idEstagio_anterior).getListaTipoSubproblemaSolver();
		else
			lista_subproblemaSolver = vetorEstagio.att(a_idEstagio).getListaTipoSubproblemaSolver();

		if ((idEstagio_anterior < estagio_inicial) && (!vetorEstagio.att(a_idEstagio).isSolverInstanciado(TipoSubproblemaSolver_mestre)))
			return;

		if (lista_subproblemaSolver.size() == 0)
			return;

		selecionarCorteBenders(a_idEstagio);

		a_entradaSaidaDados.setDiretorioSaida(a_diretorio);

		if (a_idProcesso == IdProcesso_mestre) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool())) {

				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_dominantes_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, IdCorteBenders_Nenhum, *this, std::vector<AttMatrizCorteBenders>{AttMatrizCorteBenders_custo_cortes_dominantes, AttMatrizCorteBenders_ordenacao_cortes_dominantes});

				a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_request_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, std::vector<AttVetorEstagio>{AttVetorEstagio_request_cortes_selecionados});

				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_selecionados_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, std::vector<AttMatrizEstagio>{AttMatrizEstagio_cortes_selecionados, AttMatrizEstagio_acao_cortes_selecionados});
				
				a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_deselecionados_" + getFullString(a_idEstagio) + ".csv", a_idEstagio, *this, AttMatrizEstagio_cortes_deselecionados);

			} // if (a_modeloOtimizacao.getAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, bool())) {

		}

		if (getSize1Matriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados) > 0) {

			const IdRealizacao maiorIdRealizacao = getIterador1Final(a_idEstagio, AttMatrizEstagio_cortes_selecionados, IdRealizacao());

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

				const int request_cortes_selecionados = getElementoVetor(a_idEstagio, AttVetorEstagio_request_cortes_selecionados, idRealizacao, int());

				if (request_cortes_selecionados == 1) {

					const int numero_cortes_selecionados = getIterador2Final(a_idEstagio, AttMatrizEstagio_cortes_selecionados, idRealizacao, int());

					for (int i = 1; i <= numero_cortes_selecionados; i++) {

						const int acao_corte_selecionado = getElementoMatriz(a_idEstagio, AttMatrizEstagio_acao_cortes_selecionados, idRealizacao, i, int());

						if (acao_corte_selecionado == 0) {

							const IdCorteBenders idCorteBenders = getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados, idRealizacao, i, IdCorteBenders());

							const IdRealizacao menorIdRealizacao_corte = getIterador1Inicial(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
							const IdRealizacao maiorIdRealizacao_corte = getIterador1Final(a_idEstagio, idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

							if ((menorIdRealizacao_corte <= idRealizacao) && (idRealizacao <= maiorIdRealizacao_corte)) {

								double rhs = NAN;
								SmartEnupla<IdVariavelEstado, double> coeficiente;

								requestCorteBenders(a_idEstagio, idRealizacao, idCorteBenders, rhs, coeficiente);

								if (coeficiente.size() > 0) {
									for (int i = 0; i < int(lista_subproblemaSolver.size()); i++) {
										if ((idEstagio_anterior >= estagio_inicial) && (lista_subproblemaSolver.at(i) != TipoSubproblemaSolver_mestre))
											addCorteBendersToZF(lista_subproblemaSolver.at(i), a_idEstagio, idEstagio_anterior, idRealizacao, idCorteBenders, rhs, coeficiente);
										if (lista_subproblemaSolver.at(i) == TipoSubproblemaSolver_mestre)
											addCorteBendersToZT(lista_subproblemaSolver.at(i), a_idEstagio, idRealizacao, idCorteBenders, rhs, coeficiente);
									}
								} // if (coeficiente.size() > 0) {

							} // if ((menorIdRealizacao_corte <= idRealizacao) && (idRealizacao <= maiorIdRealizacao_corte)) {

						} // if (acao_corte_selecionado > -1) {

					} // for (int i = 1; i <= numero_cortes_selecionados; i++) {

					if (getSize2Matriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao) > 0) {

						const int numero_cortes_deselecionados = getIterador2Final(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, int());

						for (int i = 1; i <= numero_cortes_deselecionados; i++) {
							for (int j = 0; j < int(lista_subproblemaSolver.size()); j++) {
								if ((idEstagio_anterior >= estagio_inicial) && (lista_subproblemaSolver.at(j) != TipoSubproblemaSolver_mestre))
									remCorteBendersFromZF(lista_subproblemaSolver.at(j), a_idEstagio, idEstagio_anterior, idRealizacao, getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, i, IdCorteBenders()));
								if (lista_subproblemaSolver.at(j) == TipoSubproblemaSolver_mestre)
									remCorteBendersFromZT(lista_subproblemaSolver.at(j), a_idEstagio, idRealizacao, getElementoMatriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao, i, IdCorteBenders()));
							}
						}
					} // if (getSize2Matriz(a_idEstagio, AttMatrizEstagio_cortes_deselecionados, idRealizacao) > 0) {

					requestCorteBendersFinalizado(a_idEstagio, idRealizacao);

				} // if (request_cortes_selecionados == 1) {

			} // for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao; idRealizacao++) {

		} // if (getSize1Matriz(a_idEstagio, AttMatrizEstagio_cortes_selecionados) > 0) {

		if ((getAtributo(AttComumModeloOtimizacao_remover_cortes_dominados, bool())) && (a_idEstagio != getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio())))
			removerCortesBendersDominados(a_idEstagio, a_idProcesso, a_entradaSaidaDados);

	} // try {
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const std::string a_diretorio, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao, const IdCorteBenders a_idCorteBenders, double & a_rhs, SmartEnupla<IdVariavelEstado, double>& a_coeficiente){

	try { vetorEstagio.att(a_idEstagio).requestCorteBenders(a_idRealizacao, a_idCorteBenders, a_rhs, a_coeficiente); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBenders(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "," + getFullString(a_idCorteBenders) + ",a_rhs,a_coeficiente,a_estado): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBenders(const IdEstagio a_idEstagio, double & a_rhs, const SmartEnupla<IdVariavelEstado, double>& a_coeficiente){


void ModeloOtimizacao::requestCorteBendersFinalizado(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){

	try { vetorEstagio.att(a_idEstagio).requestCorteBendersFinalizado(a_idRealizacao); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::requestCorteBendersFinalizado(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::requestCorteBendersFinalizado(const IdRealizacao a_idRealizacao){

void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){
	try { vetorEstagio.att(a_idEstagio).selecionarCorteBenders(); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::selecionarCorteBenders(" + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::selecionarCorteBenders(const IdEstagio a_idEstagio){



void ModeloOtimizacao::gerarRealizacoes(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados){

	try {

		if (!getAtributo(AttComumModeloOtimizacao_calcular_cenario_hidrologico_pre_otimizacao, bool()))
			return;

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		realizacoes = SmartEnupla<IdProcessoEstocastico, SmartEnupla<IdProcesso, SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>>>();

		realizacoes.addElemento(tipo_processo_estocastico_hidrologico, SmartEnupla<IdProcesso, SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>>(IdProcesso_mestre, std::vector<SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>>(a_maior_processo, SmartEnupla<IdVariavelAleatoria, SmartEnupla<IdCenario, SmartEnupla<Periodo, double>>>())));

		const bool imprimir_cenarios = getAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, bool());

		if ((getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_truncamento) \
			|| (getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento) || \
			(getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao) || \
			((getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()) == TipoRelaxacaoAfluenciaIncremental_penalizacao) && imprimir_cenarios)) {
			int semente_geracao_cenario_hidrologico = -1;
			a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//ProcessoEstocasticoHidrologico//" + getFullString(a_idProcesso));

			bool gerar_cenarios_internos = false;
			if (imprimir_cenarios)
				gerar_cenarios_internos = true;

			realizacoes.at(tipo_processo_estocastico_hidrologico).at(a_idProcesso) = vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico).gerarCenariosPorSorteioRetorno(a_entradaSaidaDados, imprimir_cenarios, true, gerar_cenarios_internos, getAtributo(AttComumModeloOtimizacao_numero_cenarios_tendencia_hidrologica, int()), getAtributo(AttComumModeloOtimizacao_numero_cenarios, int()), a_cenario_inicial, a_cenario_final, TipoSorteio_uniforme, semente_geracao_cenario_hidrologico);
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::gerarRealizacoes(" + getFullString(a_idProcesso) + "," + getFullString(a_cenario_inicial) + "," + getFullString(a_cenario_final) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::gerarRealizacaoInterna(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdCenario a_idCenario){

	try {

		EstruturaResultados<double> retorno_estados;

		atualizarModeloOtimizacaoComVariavelEstado(a_idEstagio, a_idProcesso, a_maiorIdProcesso, a_idCenario, retorno_estados);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(a_idProcesso) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdCenario a_idCenario, EstruturaResultados<double>& a_retorno_estados){

	try {

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		if (maiorIdVariavelEstado == IdVariavelEstado_Nenhum)
			return;

		const IdEstagio estagio_anterior = IdEstagio(a_idEstagio - 1);

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			const TipoSubproblemaSolver tSS_anterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_tipoSubproblemaSolverEstagioAnterior, TipoSubproblemaSolver());

			const double valor = getElementoMatriz(a_idEstagio, idVariavelEstado, AttMatrizVariavelEstado_valor, a_idProcesso, a_idCenario, double());

			if (a_retorno_estados.total > 0)
				a_retorno_estados.add(valor);

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.att(a_idEstagio).isSolverInstanciado(tSS))) {
					if (getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int()) == -1)
						vetorEstagio.att(a_idEstagio).addVariavelEstado(tSS, getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, string()), addVarDecisao_ESTADO(tSS, a_idEstagio, idVariavelEstado, 0.0, 0.0, 0.0), -1);
				} // if ((tSS == TipoSubproblemaSolver_mestre) && (vetorEstagio.att(a_idEstagio).isSolverInstanciado(tSS))) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

				} // if (idVariavelDecisao > -1) {

			}  // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(a_idProcesso) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelEstado(const IdEstagio a_idEstagio, const IdProcesso a_idProcesso, const IdCenario a_idCenario, int& a_indice, double* a_array){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

	try {

		const IdVariavelRealizacao maiorIdVariavelRealizacao = getMaiorId(a_idEstagio, IdVariavelRealizacao());

		for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

			const Periodo periodo_horizonte_processo_estocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_periodo, Periodo());

			const IdVariavelAleatoria   idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idVariavelAleatoria, IdVariavelAleatoria());

			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idProcessoEstocastico, IdProcessoEstocastico());

			const IdRealizacao idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo_horizonte_processo_estocastico, IdRealizacao());

			const double valor = getElementoMatriz(idProcessoEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_horizonte_processo_estocastico, idRealizacao, double());

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacao, AttVetorVariavelRealizacao_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (maiorIdVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->atualizar();

				} // if (idVariavelDecisao > -1) {

			} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacao(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){

	try {

		const IdVariavelRealizacao maiorIdVariavelRealizacao = getMaiorId(a_idEstagio, IdVariavelRealizacao());

		for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

			const Periodo periodo_horizonte_processo_estocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_periodo, Periodo());

			const IdVariavelAleatoria     idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idVariavelAleatoria, IdVariavelAleatoria());
			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacao, AttComumVariavelRealizacao_idProcessoEstocastico, IdProcessoEstocastico());

			const double valor = getElementoMatriz(idProcessoEstocastico, idVariavelAleatoria, AttMatrizVariavelAleatoria_residuo_espaco_amostral, periodo_horizonte_processo_estocastico, a_idRealizacao, double());

			for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

				const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacao, AttVetorVariavelRealizacao_idVariavelDecisao, tSS, int());

				if (idVariavelDecisao > -1) {

					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimInferior(idVariavelDecisao, valor);
					vetorEstagio.att(a_idEstagio).getSolver(tSS)->setLimSuperior(idVariavelDecisao, valor);

					if (maiorIdVariavelRealizacao == maiorIdVariavelRealizacao)
						vetorEstagio.att(a_idEstagio).getSolver(tSS)->atualizar();

				} // if (idVariavelDecisao > -1) {

			} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++) {

		} // for (IdVariavelRealizacao idVariavelRealizacao = IdVariavelRealizacao_1; idVariavelRealizacao <= maiorIdVariavelRealizacao; idVariavelRealizacao++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacao(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacao(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao){


bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS_destino, const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio) {
	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());

		if (maiorIdVariavelRealizacaoInterna == IdVariavelRealizacaoInterna_Nenhum)
			return true;
		
		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			const IdProcessoEstocastico           idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idProcessoEstocastico, IdProcessoEstocastico());
			const IdVariavelAleatoria               idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoria, IdVariavelAleatoria());
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

			if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

				const Periodo periodo = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_periodo, Periodo());

				double valor = NAN;

				if (a_idRealizacao == IdRealizacao_Nenhum)
					valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario).at(periodo));
				else
					valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idRealizacao, periodo, realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario)));

				double valor_novo = 0.0;

				if (valor < 1.0)
					valor_novo = 1.0 - valor;

				const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

				//vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao_origem, 0.0);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao_origem, valor_novo);

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->atualizar();

			} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {
			 
		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_nao_otimalidade) {
			//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
			otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);
		}

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

				VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

					RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
					RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
					RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
					RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
					RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

					RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
					RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
					RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
					RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
					RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7)

			} // if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao == IdRealizacao_Nenhum)) {

			else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

					RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
					RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
					RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
					RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
					RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

					RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
					RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
					RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
					RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
					RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7)

			} // else if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()) && (a_idRealizacao != IdRealizacao_Nenhum)) {

			if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {
				criarDiretorio(a_diretorio + "//" + getString(a_TSS));
				if (a_idRealizacao == IdRealizacao_Nenhum)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				else
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
			} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, bool()))) {

			for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

				const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

				if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

					const int idVariavelDecisao_origem = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

					double valor = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(idVariavelDecisao_origem);

					if (valor > 0.0) {

						const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS_destino, int());

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->setLimInferior(idVariavelDecisao, valor + 1.0);
						vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->setLimSuperior(idVariavelDecisao, valor + 1.0);

					} // if (valor > 0.0) {

				} // if (idVariavelAleatoriaInterna > IdVariavelAleatoriaInterna_Nenhum) {

				if (idVariavelRealizacaoInterna == maiorIdVariavelRealizacaoInterna)
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS_destino)->atualizar();

			} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			return true;

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(" + getFullString(a_TSS_destino) + "," + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const TipoSubproblemaSolver a_TSS_origem, const IdEstagio a_idEstagio, const IdCenario a_idCenario) {


bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao) {

	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());

		if (maiorIdVariavelRealizacaoInterna == IdVariavelRealizacaoInterna_Nenhum)
			return true;

		const double valor_viabilidade = 1.0;

		const TipoRelaxacaoAfluenciaIncremental tipoRelaxacaoAfluenciaIncremental = getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental());

		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

			const IdProcessoEstocastico           idProcessoEstocastico = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idProcessoEstocastico, IdProcessoEstocastico());
			const IdVariavelAleatoria               idVariavelAleatoria = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoria, IdVariavelAleatoria());
			const IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_idVariavelAleatoriaInterna, IdVariavelAleatoriaInterna());

			const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelRealizacaoInterna, AttVetorVariavelRealizacaoInterna_idVariavelDecisao, a_TSS, int());

			const Periodo periodo = getAtributo(a_idEstagio, idVariavelRealizacaoInterna, AttComumVariavelRealizacaoInterna_periodo, Periodo());

			double valor = 0.0;
			double novo_valor_inf = 0.0;
			double novo_valor_sup = 0.0;

			if (idVariavelAleatoriaInterna != IdVariavelAleatoriaInterna_Nenhum) {

				if ((tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao) && (a_idRealizacao > IdRealizacao_Nenhum)) {

					novo_valor_inf = 0.0;
					novo_valor_sup = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getInfinito();

				}

				else if ((((tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento) || (tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao)) && (a_idRealizacao == IdRealizacao_Nenhum)) \
					|| ((getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, periodo, idProcessoEstocastico, idVariavelAleatoria) == -1) && ((tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_truncamento) || (tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento)))) {

					if (a_idRealizacao == IdRealizacao_Nenhum)
						valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario).at(periodo));
					else
						valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoInterna(idVariavelAleatoria, idVariavelAleatoriaInterna, periodo, vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idRealizacao, periodo, realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario)));

					if (valor < valor_viabilidade) {
						novo_valor_inf = valor_viabilidade - valor;
						novo_valor_sup = novo_valor_inf;
					}

				} // if (getVarDecisao_YPFseExistir(a_TSS, a_idEstagio, periodo, idProcessoEstocastico, idVariavelAleatoria) == -1) {

			} // if (idVariavelAleatoriaInterna != IdVariavelAleatoriaInterna_Nenhum) {

			else if (idVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum) {

				if (((tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento) || (tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao)) && (a_idRealizacao == IdRealizacao_Nenhum)){}
				else {

					if (a_idRealizacao == IdRealizacao_Nenhum)
						valor = realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario).at(periodo);
					else
						valor = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacao(idVariavelAleatoria, a_idRealizacao, periodo, realizacoes.at(idProcessoEstocastico).at(a_idProcesso).at(idVariavelAleatoria).at(a_idCenario));

					const double valor_minimo_convexo = vetorProcessoEstocastico.att(idProcessoEstocastico).calcularRealizacaoParaValor(idVariavelAleatoria, 0.0, periodo);

					if (valor < valor_minimo_convexo) {
						novo_valor_sup = valor_viabilidade + valor_minimo_convexo - valor;
						//novo_valor_inf = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimInferior(idVariavelDecisao);
					}
				}

			} // else if (idVariavelAleatoriaInterna == IdVariavelAleatoriaInterna_Nenhum) {

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimInferior(idVariavelDecisao, novo_valor_inf);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setLimSuperior(idVariavelDecisao, novo_valor_sup);

		} // for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++) {

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarModeloOtimizacaoComVariavelRealizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }


} // bool ModeloOtimizacao::atualizarModeloOtimizacaoComVariavelRealizacaoInterna(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


void ModeloOtimizacao::resetarVariavelRealizacaoInterna(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		const IdVariavelRealizacaoInterna maiorIdVariavelRealizacaoInterna = getMaiorId(a_idEstagio, IdVariavelRealizacaoInterna());
		for (IdVariavelRealizacaoInterna idVariavelRealizacaoInterna = IdVariavelRealizacaoInterna_1; idVariavelRealizacaoInterna <= maiorIdVariavelRealizacaoInterna; idVariavelRealizacaoInterna++)
			vetorEstagio.att(a_idEstagio).resetarValorVariavelRealizacaoInterna(a_TSS, idVariavelRealizacaoInterna);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::resetarVariavelRealizacaoInterna(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::resetarVariavelRealizacaoInterna(){


void ModeloOtimizacao::atualizarVariavelEstadoComSolucaoPrimal(const bool a_resetar, const IdIteracao a_idIteracao, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdEstagio a_idEstagio, const IdCenario a_idCenario){

	try {

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdEstagio idEstagio_anterior = IdEstagio(a_idEstagio - 1);

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				try {

					bool resetar = false;
					if ((a_resetar) && (idVariavelEstado == IdVariavelEstado_1))
						resetar = true;

					const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

					if (idVariavelDecisaoEstagioAnterior > 0) {

						double solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getValorPrimal(idVariavelDecisaoEstagioAnterior);

						if (solucao_primal > vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);

						else if (solucao_primal < vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior))
							solucao_primal = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

						vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, resetar, a_idProcesso, a_maior_processo, a_idCenario, solucao_primal);

					} // if (idVariavelDecisaoEstagioAnterior > 0) {

				} // try
				catch (const std::exception& erro) {

					const std::string erro_str = std::string(erro.what());

					if (erro_str.size() > 5) {
						if (erro_str.find("10005") != std::string::npos) {

							escreverLog10005(tSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

								if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

									bool resetar = false;
									if ((a_resetar) && (idVariavelEstado == IdVariavelEstado_1))
										resetar = true;

									const int idVariavelDecisaoEstagioAnterior = getAtributo(a_idEstagio, idVariavelEstado, AttComumVariavelEstado_idVariavelDecisaoEstagioAnterior, int());

									if (idVariavelDecisaoEstagioAnterior > 0) {

										const double limite_superior = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimSuperior(idVariavelDecisaoEstagioAnterior);
										const double limite_inferior = vetorEstagio.att(idEstagio_anterior).getSolver(tSS)->getLimInferior(idVariavelDecisaoEstagioAnterior);

										const double valor = (limite_superior - limite_inferior) / 2.0;

										vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, resetar, a_idProcesso, a_maior_processo, a_idCenario, valor);

									} // if (idVariavelDecisaoEstagioAnterior > 0) {

								} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {
							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

						} // if (erro_str.find("10005") != std::string::npos) {					
					} // if (erro_str.size() > 5) {

					return;

				} // catch (const std::exception & erro) { 

			} // if (vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::atualizarVariavelEstadoComSolucaoPrimal(" + getFullString(a_idProcesso) + "," + getFullString(a_maior_processo) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::atualizarVariavelEstadoComSolucao(const IdEstagio a_idEstagio, const IdCenario a_idCenario){

void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_idCenario, const double valor){
	try {

		vetorEstagio.att(a_idEstagio).addValorVariavelEstado(idVariavelEstado, false, a_idProcesso, a_maior_processo, a_idCenario, valor);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::addValorVariavelEstado(" + getFullString(a_idEstagio) + "," + getFullString(idVariavelEstado) + "," + getFullString(a_idProcesso) + "," + getFullString(a_maior_processo) + "," + getFullString(a_idCenario) + "," + ",double): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::addValorVariavelEstado(const IdEstagio a_idEstagio, const IdVariavelEstado idVariavelEstado, const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdCenario a_idCenario, const double valor){


void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++)
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estagio.csv", vetorEstagio.att(idEstagio));

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirCorteBenders(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirEstagios(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirSolver(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const std::string a_nome_arquivo) {

	try {

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_nome_arquivo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolver(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + a_nome_arquivo + "): \n" + std::string(erro.what())); }


}


void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelEstado()) > IdVariavelEstado_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_estado_variaveis.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelEstado_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisEstado(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacao()) > IdVariavelRealizacao_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_realizacao.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_realizacao_variaveis.csv", IdVariavelRealizacao_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelRealizacao_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacao(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados) {

	const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
	const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

	for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
		if (getMaiorId(idEstagio, IdVariavelRealizacaoInterna()) > IdVariavelRealizacaoInterna_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(idEstagio) + "_realizacao_interna.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.att(idEstagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(idEstagio) + "_realizacao_interna_variaveis.csv", IdVariavelRealizacaoInterna_Nenhum, vetorEstagio.att(idEstagio), AttVetorVariavelRealizacaoInterna_idVariavelDecisao);
		}
	}

} // void ModeloOtimizacao::imprimirVariaveisRealizacaoInterna(EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		for (IdEstagio idEstagio = a_estagio_inicial; idEstagio <= a_estagio_final; idEstagio++)
			exportarCorteBenders(a_idProcesso, idEstagio, a_entradaSaidaDados);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::exportarCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio_saida = a_entradaSaidaDados.getDiretorioSaida();

		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes");
		else
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes//" + getFullString(a_idProcesso));

		a_entradaSaidaDados.setAppendArquivo(false);

		if (vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estagio.csv", vetorEstagio.att(a_estagio), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(a_estagio));
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_rhs.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttVetorCorteBenders_rhs);
			a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_estado.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttVetorCorteBenders_estado);
			a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_estagio) + "_corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, vetorEstagio.att(a_estagio), AttMatrizCorteBenders_coeficiente);
		} // if (vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::exportarVersaoAlternativaCorteBenders(const IdProcesso a_idProcesso, const IdEstagio a_estagio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdCorteBenders maiorIdCorteBenders = vetorEstagio.att(a_estagio).vetorCorteBenders.getMaiorId();

		if (maiorIdCorteBenders == IdCorteBenders_Nenhum)
			return;

		if (a_estagio > getAtributo(AttComumModeloOtimizacao_maior_estagio_impressao_versao_alternativa_cortes, IdEstagio()))
			return;

		const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		if (idProcessoEstocastico != IdProcessoEstocastico_hidrologico_bacia)
			return;

		const IdProcessoEstocastico idProcessoEstocasticoNovo = IdProcessoEstocastico_hidrologico_hidreletrica;
		const std::string idProcessoEstocasticoNovo_str = getFullString(idProcessoEstocasticoNovo);
		const std::string idProcessoEstocasticoNovo_compacto_str = getString(idProcessoEstocasticoNovo);

		const IdVariavelAleatoria maiorIdVariavelAleatoria = getMaiorId(idProcessoEstocastico, IdVariavelAleatoria());

		SmartEnupla<IdVariavelAleatoria, std::vector<std::string>> lista_nome_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<std::string>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<std::string>()));
		SmartEnupla<IdVariavelAleatoria, std::vector<std::string>> lista_nome_compacto_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<std::string>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<std::string>()));
		SmartEnupla<IdVariavelAleatoria, std::vector<double>>      lista_participacao_idVariavelAleatoriaNova(IdVariavelAleatoria_1, std::vector<std::vector<double>>(int(maiorIdVariavelAleatoria - IdVariavelAleatoria_1)+ 1, std::vector<double>()));

		const SmartEnupla<Periodo, double> horizonte_processo_estocastico_hidrologico = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_processo_estocastico_hidrologico, a_estagio, Periodo(), double());

		const Periodo periodo_inicial_otimizacao = horizonte_processo_estocastico_hidrologico.getIteradorInicial();
		const Periodo periodo_final_otimizacao = horizonte_processo_estocastico_hidrologico.getIteradorFinal();

		const Periodo periodo_otimizacao = getAtributo(a_estagio, AttComumEstagio_periodo_otimizacao, Periodo());


		for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcessoEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

			for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(idProcessoEstocastico, idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

				const std::string nome = getAtributo(idProcessoEstocastico, idVariavelAleatoria, idVariavelAleatoriaInterna, AttComumVariavelAleatoriaInterna_nome, std::string());

				const IdVariavelAleatoria idVariavelAleatoriaNova = getIdVariavelAleatoriaFromChar(nome.substr(nome.find("_") + 1).c_str());

				if (idVariavelAleatoriaNova == IdVariavelAleatoria_Nenhum)
					throw std::invalid_argument("Erro ao encontrar variavel aleatoria por hidreletrica em " + getFullString(idProcessoEstocastico) + "," + getFullString(idVariavelAleatoria) + "," + getFullString(idVariavelAleatoriaInterna));


				lista_nome_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(getFullString(idVariavelAleatoriaNova));
				lista_nome_compacto_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(getString(idVariavelAleatoriaNova));
				
				double participacao = 0.0;
				for (Periodo periodo = periodo_inicial_otimizacao; periodo <= periodo_final_otimizacao; horizonte_processo_estocastico_hidrologico.incrementarIterador(periodo)) {
					participacao += double(periodo.getSegundos() / periodo_otimizacao.getSegundos()) * getElementoVetor(idProcessoEstocastico, idVariavelAleatoria, idVariavelAleatoriaInterna, AttVetorVariavelAleatoriaInterna_coeficiente_participacao, periodo_inicial_otimizacao, double());
				}

				lista_participacao_idVariavelAleatoriaNova.at(idVariavelAleatoria).push_back(participacao);

			} // for (IdVariavelAleatoriaInterna idVariavelAleatoriaInterna = IdVariavelAleatoriaInterna_1; idVariavelAleatoriaInterna <= getMaiorId(a_tipo_processo_estocastico_hidrologico, idVariavelAleatoria, IdVariavelAleatoriaInterna()); idVariavelAleatoriaInterna++) {

		} // for (IdVariavelAleatoria idVariavelAleatoria = IdVariavelAleatoria_1; idVariavelAleatoria <= getMaiorId(idProcessoEstocastico, IdVariavelAleatoria()); idVariavelAleatoria++) {

		Estagio estagio;

		vetorEstagio.att(a_estagio).exportarVersaoAlternativaCorteBenders(estagio, idProcessoEstocasticoNovo_str, idProcessoEstocasticoNovo_compacto_str, lista_nome_idVariavelAleatoriaNova, lista_nome_compacto_idVariavelAleatoriaNova, lista_participacao_idVariavelAleatoriaNova);

		const std::string diretorio_saida = a_entradaSaidaDados.getDiretorioSaida();

		if (a_idProcesso == IdProcesso_mestre)
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes_" + getString(idProcessoEstocasticoNovo));
		else
			a_entradaSaidaDados.setDiretorioSaida(diretorio_saida + "//Cortes" + getString(idProcessoEstocasticoNovo) + "//" + getFullString(a_idProcesso));

		a_entradaSaidaDados.setAppendArquivo(false);

		a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estagio.csv", estagio, std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
		a_entradaSaidaDados.imprimirArquivoCSV_AttComum(getFullString(a_estagio) + "_estado.csv", IdVariavelEstado_Nenhum, estagio);
		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_rhs.csv", IdCorteBenders_Nenhum, estagio, AttVetorCorteBenders_rhs);
		a_entradaSaidaDados.imprimirArquivoCSV_AttVetor(getFullString(a_estagio) + "_corteBenders_estado.csv", IdCorteBenders_Nenhum, estagio, AttVetorCorteBenders_estado);
		a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz(getFullString(a_estagio) + "_corteBenders_coeficientes.csv", IdCorteBenders_Nenhum, estagio, AttMatrizCorteBenders_coeficiente);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVersaoAlternativaCorteBenders(" + getFullString(a_idProcesso) + "," + getFullString(a_estagio) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::exportarVersaoAlternativaCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final, EntradaSaidaDados a_entradaSaidaDados){

void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio_inicial, const IdEstagio a_estagio_final) {

	try {

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++)
			removerCorteBenders(idEstagio);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCorteBenders(" + getFullString(a_estagio_inicial) + "," + getFullString(a_estagio_final) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){

void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){

	try {

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_estagio, IdCorteBenders()); idCorteBenders++) {
			if (vetorEstagio.att(a_estagio).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.att(a_estagio).vetorCorteBenders.rem(idCorteBenders);
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(a_estagio, IdCorteBenders()); idCorteBenders++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::removerCorteBenders(" + getFullString(a_estagio) + "): \n" + std::string(erro.what())); }


} // void ModeloOtimizacao::removerCorteBenders(const IdEstagio a_estagio){


void ModeloOtimizacao::exportarCorteBenders_AcoplamentoPreEstudo(const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPreEstudo");

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())){

			if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

				for (IdCorteBenders idCorteBenders = IdCorteBenders(maiorIdCorteExportadoPreEstudo + 1); idCorteBenders <= getMaiorId(estagio_acoplamento_pre_estudo, IdCorteBenders()); idCorteBenders++) {
					if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.isInstanciado(idCorteBenders)) {
						if (idCorteBenders == IdCorteBenders(maiorIdCorteExportadoPreEstudo + 1))
							a_entradaSaidaDados.setAppendArquivo(false);
						else
							a_entradaSaidaDados.setAppendArquivo(true);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_rhs_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttVetorCorteBenders_rhs);
						a_entradaSaidaDados.imprimirArquivoCSV_AttVetor("corteBenders_estado_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttVetorCorteBenders_estado);
						a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("corteBenders_coeficientes_" + getFullString(a_idIteracao) + ".csv", idCorteBenders, vetorEstagio.att(estagio_acoplamento_pre_estudo), AttMatrizCorteBenders_coeficiente);
					}
				}

				a_entradaSaidaDados.setAppendArquivo(false);
				a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

				maiorIdCorteExportadoPreEstudo = getMaiorId(estagio_acoplamento_pre_estudo, IdCorteBenders());

				if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao()))
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("eop.txt", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

			} // if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorCorteBenders.getMaiorId() != IdCorteBenders_Nenhum) {

		} // if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirCorteBendersAcoplamento(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirCorteBendersAcoplamento(EntradaSaidaDados a_entradaSaidaDados, const bool a_imprimir_estagio_e_estados){


void ModeloOtimizacao::importarCorteBenders(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico) {

	try {

		const string diretorio_importacao_cortes = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes, std::string());

		if (strCompara(diretorio_importacao_cortes, "nenhum"))
			return;

		if (a_TSS == TipoSubproblemaSolver_viabilidade_hidraulica)
			return;

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		VetorEstagioEmModeloOtimizacao vetorEstagio_aux;

		const IdCorteBenders maximoIdCorteBenders = getAtributo(AttComumModeloOtimizacao_maior_corte_importado, IdCorteBenders());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_cortes);
		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {
			if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {
				vetorEstagio.att(idEstagio).vetorCorteBenders.alocar(int(maximoIdCorteBenders));
				Estagio estagio_aux;
				estagio_aux.setAtributo(AttComumEstagio_idEstagio, idEstagio);
				vetorEstagio_aux.add(estagio_aux);
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idEstagio) + "_estagio.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_direto);
				a_entradaSaidaDados.carregarArquivoCSV_AttVetor(getFullString(idEstagio) + "_corteBenders_rhs.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_membro);
				vetorEstagio_aux.att(idEstagio).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + getFullString(idEstagio) + "_corteBenders_estado.csv");
				vetorEstagio_aux.att(idEstagio).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//" + getFullString(idEstagio) + "_corteBenders_coeficientes.csv");
				a_entradaSaidaDados.carregarArquivoCSV_AttComum(getFullString(idEstagio) + "_estado.csv", vetorEstagio_aux.att(idEstagio), TipoAcessoInstancia_membro);
			} // if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {
		} // for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

		for (IdEstagio idEstagio = estagio_inicial; idEstagio <= estagio_final; idEstagio++) {

			if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

				if (getAtributo(idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()).sobreposicao(vetorEstagio_aux.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) == 0.0)
					throw std::invalid_argument("Periodo " + getString(vetorEstagio_aux.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) + " importado nao compativel com periodo do estagio " + getFullString(idEstagio));

				const IdCorteBenders maiorIdCorteBenders = vetorEstagio_aux.att(idEstagio).getMaiorId(IdCorteBenders());

				const IdVariavelEstado maiorIdVarivelEstado = getMaiorId(idEstagio, IdVariavelEstado());

				IdRealizacao maiorIdRealizacao_corte = IdRealizacao_Nenhum;

				IdVariavelEstado maiorIdVarivelEstado_corte = IdVariavelEstado_Nenhum;
				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorEstagio_aux.att(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders)) {
						const IdRealizacao idRealizacao = vetorEstagio_aux.att(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
						if (idRealizacao > maiorIdRealizacao_corte)
							maiorIdRealizacao_corte = idRealizacao;
						if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
							maiorIdVarivelEstado_corte = vetorEstagio_aux.att(idEstagio).getIterador2Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, IdVariavelEstado());
					}
				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

				if (maiorIdVarivelEstado_corte == IdVariavelEstado_Nenhum)
					throw std::invalid_argument("Nenhum corte foi importado em " + getFullString(idEstagio));

				const int cortes_multiplos_modelo = getElementoVetor(AttVetorModeloOtimizacao_cortes_multiplos, idEstagio, int());

				if ((cortes_multiplos_modelo == 0) && (maiorIdRealizacao_corte != IdRealizacao_1))
					throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com a multiplicidade do modelo em " + getFullString(idEstagio));

				else if ((cortes_multiplos_modelo > 0) && (maiorIdRealizacao_corte > getAtributo(idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao())))
					throw std::invalid_argument("A multiplicidade dos cortes importados nao eh compativel com o numero de aberturas do modelo em " + getFullString(idEstagio));

				int numero_variaveis_estado_modelo_encontradas = 0;
				int numero_variaveis_estado_cortes_encontradas = 0;
				SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_modelo_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado,       IdVariavelEstado_Nenhum));
				SmartEnupla<IdVariavelEstado, IdVariavelEstado> variaveis_estado_cortes_encontradas(IdVariavelEstado_1, std::vector<IdVariavelEstado>(maiorIdVarivelEstado_corte, IdVariavelEstado_Nenhum));

				bool modelo_e_cortes_estados_identicos = true;

				for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

					IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_Nenhum;

					try {

						if (idVariavelEstado <= maiorIdVarivelEstado_corte) {

							if (!strCompara(getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()), vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()))) {

								modelo_e_cortes_estados_identicos = false;

								for (idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {


									if (strCompara(getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()), vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()))) {
										variaveis_estado_modelo_encontradas.at(idVariavelEstado) = idVariavelEstado_corte;
										variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) = idVariavelEstado;
										numero_variaveis_estado_modelo_encontradas++;
										numero_variaveis_estado_cortes_encontradas++;
										break;
									}

								} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

							} // if (!strCompara(getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()), vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()))) {
							else {
								variaveis_estado_modelo_encontradas.at(idVariavelEstado) = idVariavelEstado;
								variaveis_estado_cortes_encontradas.at(idVariavelEstado) = idVariavelEstado;
								numero_variaveis_estado_modelo_encontradas++;
								numero_variaveis_estado_cortes_encontradas++;
							}
						} // if (idVariavelEstado <= maiorIdVarivelEstado_corte) {

					} // try {

					catch (const std::exception& erro) { 
						throw std::invalid_argument("Erro " + getFullString(idEstagio) + ", " + getFullString(idVariavelEstado) + ", Corte: " + getFullString(idVariavelEstado_corte) + ", \n" + std::string(erro.what()));
					}


				} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

				//
				// Tratamento de vari�veis do modelo que n�o foram encontradas no corte.
				// 

				if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {

					for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

						if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

							const std::vector<std::string> nome = vetorEstagio.att(idEstagio).getNomeVariavelEstado(idVariavelEstado);

							if ((nome.at(0) != "varYP") && (nome.at(0) != "varVI")) {
								throw std::invalid_argument(getFullString(idVariavelEstado) + " " + getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string()) + " presente no modelo, nao consta no corte em " + getFullString(idEstagio));
							}

						} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {

					} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {

				} // if (numero_variaveis_estado_modelo_encontradas != int(maiorIdVarivelEstado)) {


				//
				// Tratamento de vari�veis do corte que n�o foram encontradas no modelo.
				// 

				if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {

					modelo_e_cortes_estados_identicos = false;

					for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

						if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

							const std::vector<std::string> nome = vetorEstagio_aux.att(idEstagio).getNomeVariavelEstado(idVariavelEstado_corte);

							if ((nome.at(0) != "varYP") && (nome.at(0) != "varVI") && (nome.at(0) != "varVDEF")) {
								throw std::invalid_argument(getFullString(idVariavelEstado_corte) + " " + vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()) + " presente no corte, nao consta no modelo em " + getFullString(idEstagio));
							}


							if (nome.at(0) == "varVI") {

								const Periodo periodo(nome.at(2));

								const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

								if (periodo.sobreposicao(vetorEstagio.att(idEstagio).getAtributo(AttComumEstagio_periodo_otimizacao, Periodo())) == 0.0)
									throw std::invalid_argument("Periodo " + getFullString(periodo) + " nao compativel com varVI de " + getFullString(idVariavelEstado_corte) + " no corte em " + getFullString(periodo));

								Periodo periodo_estudo_inicial = a_dados.getIterador2Inicial(AttMatrizDados_percentual_duracao_horizonte_estudo, idEstagio, Periodo());

								IdVariavelEstado idVariavelEstado_encontrada = IdVariavelEstado_Nenhum;

								for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVarivelEstado; idVariavelEstado++) {
									const std::string nome_modelo = getAtributo(idEstagio, idVariavelEstado, AttComumVariavelEstado_nome, std::string());
									if (strCompara(nome_modelo, std::string("varVI," + getString(idEstagio) + "," + getString(periodo_estudo_inicial) + "," + getString(idHidreletrica)))) {
										idVariavelEstado_encontrada = idVariavelEstado;
										break;
									}
									else if (strCompara(nome_modelo, std::string("varVI," + getFullString(idEstagio) + "," + getString(periodo_estudo_inicial) + "," + getFullString(idHidreletrica)))) {
										idVariavelEstado_encontrada = idVariavelEstado;
										break;
									}
								}

								if (idVariavelEstado_encontrada != IdVariavelEstado_Nenhum)
									variaveis_estado_modelo_encontradas.setElemento(idVariavelEstado_encontrada, idVariavelEstado_corte);
								else
									vetorEstagio_aux.att(idEstagio).vetorVariavelEstado.rem(idVariavelEstado_corte);

							} // if (nome.at(0) == "varVI") {

							else if (nome.at(0) == "varYP") {

								const Periodo periodo = Periodo(nome.at(2));

								const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(3).c_str());

								if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
									throw std::invalid_argument(getFullString(idProcessoEstocastico) + " do corte incompativel com " + getFullString(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico())) + " do modelo em " + getFullString(idEstagio));

								const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(4).c_str());

								const int lag = getintFromChar(nome.at(5).c_str());

								Periodo periodo_processo_estocastico_PAR = periodo - lag;

								//vetorEstagio.att(idEstagio).addVariavelEstado(a_TSS, vetorEstagio_aux.att(idEstagio).getAtributo(idVariavelEstado_corte, AttComumVariavelEstado_nome, std::string()), -1, -1);

								IdVariavelEstado idVariavelEstado_criada = IdVariavelEstado_Nenhum;

								const int varYP = criarVariaveis_Decisao_e_Estado_YP_ACUMULADO_From_CorteBenders(a_TSS, a_dados, idEstagio, getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, IdEstagio_1, Periodo()), periodo_processo_estocastico_PAR, lag, idProcessoEstocastico, idVariavelAleatoria, a_horizonte_tendencia_mais_processo_estocastico, idVariavelEstado_criada);

								if (varYP > -1)
									variaveis_estado_modelo_encontradas.addElemento(idVariavelEstado_criada, idVariavelEstado_corte);
								//	vetorEstagio_aux.att(idEstagio).setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado_corte, a_TSS, varYP);

								else if (varYP == -1) {
									vetorEstagio_aux.att(idEstagio).vetorVariavelEstado.rem(idVariavelEstado_corte);
								} // else if (varYH == -1) {

							} // else if (nome.at(0) == "varYP") {

							else if (nome.at(0) == "varVDEF") {
								vetorEstagio_aux.att(idEstagio).vetorVariavelEstado.rem(idVariavelEstado_corte);
							} // else if (nome.at(0) == "varVDEF") {


						} // if (variaveis_estado_cortes_encontradas.at(idVariavelEstado_corte) == IdVariavelEstado_Nenhum) {

					} // for (IdVariavelEstado idVariavelEstado_corte = IdVariavelEstado_1; idVariavelEstado_corte <= maiorIdVarivelEstado_corte; idVariavelEstado_corte++) {

				} // if (numero_variaveis_estado_cortes_encontradas != int(maiorIdVarivelEstado_corte)) {


				IdCorteBenders idCorteBenders_sequencial = IdCorteBenders_Nenhum;
				for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {
					if (vetorEstagio_aux.att(idEstagio).vetorCorteBenders.isInstanciado(idCorteBenders) && (idCorteBenders <= maximoIdCorteBenders)) {
						if (idCorteBenders_sequencial == IdCorteBenders_Nenhum)
							idCorteBenders_sequencial = IdCorteBenders_1;
						else
							idCorteBenders_sequencial++;

						if (modelo_e_cortes_estados_identicos) {
							CorteBenders corteBenders(vetorEstagio_aux.att(idEstagio).vetorCorteBenders.at(idCorteBenders));
							corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);
							vetorEstagio.att(idEstagio).vetorCorteBenders.add(corteBenders);
						} // if (modelo_e_cortes_estados_identicos) {

						else if (!modelo_e_cortes_estados_identicos) {

							CorteBenders corteBenders;
							corteBenders.setAtributo(AttComumCorteBenders_idCorteBenders, idCorteBenders_sequencial);

							const IdRealizacao idRealizacao_inicial = vetorEstagio_aux.att(idEstagio).getIterador1Inicial(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
							const IdRealizacao idRealizacao_final   = vetorEstagio_aux.att(idEstagio).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());

							for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

								if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) == IdVariavelEstado_Nenhum) {
									corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, 0.0);

									for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
										corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, 0.0);

								}

								else if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {
									corteBenders.addElemento(AttVetorCorteBenders_estado, idVariavelEstado, vetorEstagio_aux.att(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_estado, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

									for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
										corteBenders.addElemento(AttMatrizCorteBenders_coeficiente, idRealizacao, idVariavelEstado, vetorEstagio_aux.att(idEstagio).getElementoMatriz(idCorteBenders, AttMatrizCorteBenders_coeficiente, idRealizacao, variaveis_estado_modelo_encontradas.at(idVariavelEstado), double()));

								} // if (variaveis_estado_modelo_encontradas.at(idVariavelEstado) != IdVariavelEstado_Nenhum) {

							} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= variaveis_estado_modelo_encontradas.getIteradorFinal(); idVariavelEstado++) {

							for (IdRealizacao idRealizacao = idRealizacao_inicial; idRealizacao <= idRealizacao_final; idRealizacao++)
								corteBenders.addElemento(AttVetorCorteBenders_rhs, idRealizacao, vetorEstagio_aux.att(idEstagio).getElementoVetor(idCorteBenders, AttVetorCorteBenders_rhs, idRealizacao, double()));

							vetorEstagio.att(idEstagio).vetorCorteBenders.add(corteBenders);

						} // else if (!modelo_e_cortes_estados_identicos) {
					}
				} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

				requestCorteBenders(a_idProcesso, idEstagio, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

				if (getAtributo(idEstagio, AttComumEstagio_selecao_cortes_nivel_dominancia, int()) == 0)
					removerCorteBenders(idEstagio);

			} // if ((estagio_inicial != IdEstagio_1) || (idEstagio > estagio_inicial)) {

		} // for (IdEstagio idEstagio = IdEstagio_1; idEstagio <= estagio_final; idEstagio++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarCorteBenders(" + getFullString(a_TSS) + "," + a_diretorio_impressao_selecao_cortes + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarCorteBenders(const std::string a_diretorio_recuperacao_cortes, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::importarCorteBenders_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const std::string a_diretorio_impressao_selecao_cortes, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pos_estudo);

		IdEstagio idEstagio_futuro = getMaiorId(IdEstagio());

		if (idEstagio_futuro != IdEstagio(estagio_final + 1))
			throw std::invalid_argument("Cortes de Benders pos-estudo devem ser importados em " + getFullString(IdEstagio(estagio_final + 1)) + ".");

		bool impressao = false;

		bool estagio_carregado_sem_iterador = false;

		bool estagio_carregado = false;
		bool rhs_carregado = false;
		bool std_carregado = false;
		bool cof_carregado = false;
		Estagio estagio_auxiliar;
		while (true) {

			if (!estagio_carregado)
				estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("estagio_" + getFullString(a_idIteracao) + ".csv", estagio_auxiliar, TipoAcessoInstancia_direto);

			if (estagio_carregado) {

				rhs_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("corteBenders_rhs_" + getFullString(a_idIteracao) + ".csv", vetorEstagio.att(idEstagio_futuro), TipoAcessoInstancia_membro);
				std_carregado = vetorEstagio.att(idEstagio_futuro).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//corteBenders_estado_" + getFullString(a_idIteracao) + ".csv");
				cof_carregado = vetorEstagio.att(idEstagio_futuro).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//corteBenders_coeficientes_" + getFullString(a_idIteracao) + ".csv");

			} // if (estagio_carregado) {

			if ((rhs_carregado) && (std_carregado) && (cof_carregado)) {
				if (a_idProcesso == IdProcesso_mestre)
					std::cout << "Cortes de Benders pos-estudo detectados." << std::endl;
				break;
			}

			if (!estagio_carregado) {
				estagio_carregado_sem_iterador = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("estagio.csv", estagio_auxiliar, TipoAcessoInstancia_direto);

				if (estagio_carregado_sem_iterador) {

					if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) {
						a_entradaSaidaDados.carregarArquivoCSV_AttVetor("corteBenders_rhs.csv", vetorEstagio.att(idEstagio_futuro), TipoAcessoInstancia_membro);
						vetorEstagio.att(idEstagio_futuro).carregarEstadosCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//corteBenders_estado.csv");
						vetorEstagio.att(idEstagio_futuro).carregarCoeficientesCortesBenders(a_entradaSaidaDados.getDiretorioEntrada() + "//corteBenders_coeficientes.csv");
					}
					else {
						return;
					}
					break;
				}

			}


			if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
				if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("eop.txt", estagio_auxiliar, TipoAcessoInstancia_direto))
					return;
				std::cout << "Aguardando Cortes de Benders no diretorio: " << diretorio_importacao_pos_estudo << std::endl;
				std::cout << "Caso NAO haja acoplamento pos-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pos_estudo) << " para: Nenhum" << std::endl;
				impressao = true;
			} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

			aguardarTempo(5000);

		} // while (true) {


		const IdCorteBenders maximoIdCorteBenders = getAtributo(AttComumModeloOtimizacao_maior_corte_importado_pos_estudo, IdCorteBenders());

		IdRealizacao maiorIdRealizacao_corte = IdRealizacao_Nenhum;

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maximoIdCorteBenders; idCorteBenders++) {
			if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders)) {
				const IdRealizacao idRealizacao = vetorEstagio.att(idEstagio_futuro).getIterador1Final(idCorteBenders, AttMatrizCorteBenders_coeficiente, IdRealizacao());
				if (idRealizacao > maiorIdRealizacao_corte)
					maiorIdRealizacao_corte = idRealizacao;
			}
		} // for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= maiorIdCorteBenders; idCorteBenders++) {

		if (maiorIdRealizacao_corte == IdRealizacao_Nenhum)
			throw std::invalid_argument("Nenhum corte foi importado.");

		int multiplicidade_corte = 0;

		if (maiorIdRealizacao_corte > IdRealizacao_1)
			multiplicidade_corte = int(maiorIdRealizacao_corte);

		if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {
			for (IdCorteBenders idCorteBenders = IdCorteBenders(maximoIdCorteBenders + 1); idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++)
				vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.rem(idCorteBenders);
		} // if (maximoIdCorteBenders < IdCorteBenders(IdCorteBenders_Excedente - 1)) {

		if (getIteradorFinal(AttVetorModeloOtimizacao_cortes_multiplos, IdEstagio()) >= idEstagio_futuro)
			setElemento(AttVetorModeloOtimizacao_cortes_multiplos, idEstagio_futuro, multiplicidade_corte);
		else if (getIteradorFinal(AttVetorModeloOtimizacao_cortes_multiplos, IdEstagio()) == IdEstagio(idEstagio_futuro - 1))
			addElemento(AttVetorModeloOtimizacao_cortes_multiplos, idEstagio_futuro, multiplicidade_corte);

		for (IdCorteBenders idCorteBenders = IdCorteBenders_1; idCorteBenders <= getMaiorId(idEstagio_futuro, IdCorteBenders()); idCorteBenders++) {
			if (vetorEstagio.att(idEstagio_futuro).vetorCorteBenders.isInstanciado(idCorteBenders))
				vetorEstagio.att(idEstagio_futuro).avaliarDominanciaCorteBendersParaEstado(idCorteBenders, idCorteBenders);
		}

		const Periodo periodo_otimizacao_estagio_final = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());

		if (getVarDecisao_ZFseExistir(a_TSS, estagio_final, periodo_otimizacao_estagio_final) == -1) {

			// Vari�vel ZF
			addVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, 0.0, vetorEstagio.att(estagio_final).getSolver(a_TSS)->getInfinito(), 0.0);

			// Vari�vel ZF em EquLinear_CUSTO_TOTAL
			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final), getEquLinear_CUSTO_TOTAL(a_TSS, estagio_final, periodo_otimizacao_estagio_final), -1.0);

			criarRestricoesCorteBendersEmCustoFuturo(a_TSS, estagio_final);

			const int posEquZF = addEquLinear_CUSTO_FUTURO(a_TSS, estagio_final, periodo_otimizacao_estagio_final);

			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setRHSRestricao(posEquZF, 0.0);

			// Vari�vel ZF
			vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final), posEquZF, 1.0);

			for (IdRealizacao idRealizacao = IdRealizacao_1; idRealizacao <= maiorIdRealizacao_corte; idRealizacao++)
				vetorEstagio.att(estagio_final).getSolver(a_TSS)->setCofRestricao(getVarDecisao_ZF(a_TSS, estagio_final, periodo_otimizacao_estagio_final, idRealizacao), posEquZF, -(1.0 / double(maiorIdRealizacao_corte)));

		} // if (getVarDecisao_ZFseExistir(estagio_final, periodo_otimizacao_estagio_final) == -1) {

		requestCorteBenders(a_idProcesso, idEstagio_futuro, a_diretorio_impressao_selecao_cortes, a_entradaSaidaDados);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarCorteBenders_AcoplamentoPosEstudo(" + getFullString(a_TSS) + "," + a_diretorio_impressao_selecao_cortes + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarCorteBenders_AcoplamentoPosEstudo(const std::string a_diretorio_recuperacao_cortes, EntradaSaidaDados a_entradaSaidaDados){


void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, const IdCenario a_cenario_inicial, const IdCenario a_cenario_final, EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pre_estudo);

		Estagio estagio;

		bool impressao = false;
		bool estado_valores_carregados = false;
		Estagio estagio_auxiliar;
		while (true) {

			estado_valores_carregados = a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir(std::string("estado_valores_" + getFullString(a_idIteracao) + ".csv"), estagio, TipoAcessoInstancia_membro);

			if (estado_valores_carregados) {
				if (a_idProcesso == IdProcesso_mestre)
					std::cout << "Variaveis Estado pre-estudo detectadas." << std::endl;
				break;
			}
			else {
				if (a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("eop.txt", estagio_auxiliar, TipoAcessoInstancia_direto))
					return;
			}


			if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
				std::cout << "Aguardando variaveis de estado no diretorio: " << diretorio_importacao_pre_estudo << std::endl;
				std::cout << "Caso NAO haja acoplamento pre-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pre_estudo) << " para: Nenhum" << std::endl;
				impressao = true;
			} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

			aguardarTempo(5000);

		} // while (true) {

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(estagio_acoplamento_pre_estudo, IdVariavelEstado()); idVariavelEstado++) {

			if (estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

				if (estagio.getSize1Matriz(idVariavelEstado, AttMatrizVariavelEstado_valor) > 0) {

					for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

						const IdCenario idCenario_pre_estudo = getElementoVetor(tipo_processo_estocastico_hidrologico, AttVetorProcessoEstocastico_mapeamento_tendencia_temporal, idCenario, IdCenario());

						IdProcesso idProcesso_busca = IdProcesso_Nenhum;

						for (IdProcesso idProcesso = estagio.getIterador1Inicial(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, IdProcesso()); idProcesso <= estagio.getIterador1Final(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, IdProcesso()); idProcesso++) {
							if (estagio.getSize2Matriz(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) {
								if ((estagio.getIterador2Inicial(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario()) <= idCenario_pre_estudo) && (idCenario_pre_estudo <= estagio.getIterador2Final(IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario()))) {
									idProcesso_busca = idProcesso;
									break;
								}
							}
						}

						vetorEstagio.att(estagio_acoplamento_pre_estudo).addValorVariavelEstado(idVariavelEstado, true, a_idProcesso, a_maior_processo, idCenario, estagio.getElementoMatriz(idVariavelEstado, AttMatrizVariavelEstado_valor, idProcesso_busca, idCenario_pre_estudo, double()));

					} // for (IdCenario idCenario = a_cenario_inicial; idCenario <= a_cenario_final; idCenario++) {

				} // if (estagio.getSize1Matriz(idVariavelEstado, AttMatrizVariavelEstado_valor) > 0) {

			} // if (estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) {

			else if ((!estagio.vetorVariavelEstado.isInstanciado(idVariavelEstado)) && (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) && (a_idProcesso == IdProcesso_mestre))
				std::cout << "Nao foi instanciado " + getFullString(idVariavelEstado) << "." << std::endl;

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= getMaiorId(estagio_acoplamento_pre_estudo, IdVariavelEstado()); idVariavelEstado++) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {


void ModeloOtimizacao::exportarVariaveisEstado_AcoplamentoPreEstudo(EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pre_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, std::string());

		if (strCompara(diretorio_importacao_pre_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPreEstudo");

		const IdEstagio estagio_acoplamento_pre_estudo = getAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, IdEstagio());

		if ((estagio_acoplamento_pre_estudo >= getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio())) && (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio()))) {

			if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorVariavelEstado.getMaiorId() != IdVariavelEstado_Nenhum) {

				if (getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()) == IdEstagio_1) {

					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(estagio_acoplamento_pre_estudo));

				}
				else {
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio_" + getFullString(getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) + ".csv", vetorEstagio.att(estagio_acoplamento_pre_estudo), std::vector<AttComumEstagio>{ AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
					a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado_" + getFullString(getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) + ".csv", IdVariavelEstado_Nenhum, vetorEstagio.att(estagio_acoplamento_pre_estudo));
				}

			} // if (vetorEstagio.att(estagio_acoplamento_pre_estudo).vetorVariavelEstado.getMaiorId() != IdVariavelEstado_Nenhum) {

		} // if (estagio_acoplamento_pre_estudo <= getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio())) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVariaveisEstado_AcoplamentoPreEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados) {


void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPosEstudo(const TipoSubproblemaSolver a_TSS, Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada, const SmartEnupla <Periodo, bool> a_horizonte_tendencia_mais_processo_estocastico) {

	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		const IdEstagio estagio_inicial = getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio());
		const IdEstagio estagio_final = getAtributo(AttComumModeloOtimizacao_estagio_final, IdEstagio());

		a_entradaSaidaDados.setDiretorioEntrada(diretorio_importacao_pos_estudo);

		const SmartEnupla<Periodo, IdEstagio> horizonte_estudo = a_dados.getVetor(AttVetorDados_horizonte_estudo, Periodo(), IdEstagio());

		// Instanciar estagio adicional a conter os cortes e estados do acoplamento.

		bool estagio_carregado_sem_iterador = false;
		bool estagio_carregado = false;

		IdEstagio idEstagio_futuro = getMaiorId(IdEstagio());

		if (estagio_final == idEstagio_futuro) {

			Estagio estagio;


			bool impressao = false;
			while (true) {

				estagio_carregado_sem_iterador = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("estagio.csv", estagio, TipoAcessoInstancia_direto);

				if (!estagio_carregado_sem_iterador)
					estagio_carregado = a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("estagio_" + getFullString(getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_direto);

				if (estagio_carregado_sem_iterador || estagio_carregado) {
					if (a_idProcesso == IdProcesso_mestre)
						std::cout << "Variaveis Estado pos-estudo detectadas." << std::endl;
					break;
				}

				if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {
					std::cout << "Aguardando variaveis de estado no diretorio: " << diretorio_importacao_pos_estudo << std::endl;
					std::cout << "Caso NAO haja acoplamento pos-estudo, pressione Ctrl+C para sair e altere valor de " << getFullString(AttComumDados_diretorio_importacao_pos_estudo) << " para: Nenhum" << std::endl;
					impressao = true;
				} // if ((!impressao) && (a_idProcesso == IdProcesso_mestre)) {

				aguardarTempo(5000);

			} // while (true) {

			if (true) {

				const Periodo periodo_otimizacao = getAtributo(estagio_final, AttComumEstagio_periodo_otimizacao, Periodo());
				const Periodo periodo_otimizacao_lido = estagio.getAtributo(AttComumEstagio_periodo_otimizacao, Periodo());

				if (Periodo(periodo_otimizacao_lido.getTipoPeriodo(), periodo_otimizacao + 1) != periodo_otimizacao_lido)
					throw std::invalid_argument("Periodo " + getString(periodo_otimizacao_lido) + " importado nao compativel com final do horizonte de otimizacao.");

			} // if (true) {

			if (estagio_carregado_sem_iterador)
				a_entradaSaidaDados.carregarArquivoCSV_AttComum("estado.csv", estagio, TipoAcessoInstancia_membro);
			else
				a_entradaSaidaDados.carregarArquivoCSV_AttComum("estado_" + getFullString(getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) + ".csv", estagio, TipoAcessoInstancia_membro);


			idEstagio_futuro = IdEstagio(estagio_final + 1);
			const IdEstagio idEstagio = estagio_final;

			estagio.setAtributo(AttComumEstagio_idEstagio, idEstagio_futuro);
			estagio.setAtributo(AttComumEstagio_selecao_cortes_nivel_dominancia, getAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, int()));

			const Periodo periodo_estudo_inicial = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());
			const Periodo periodo_estudo_final = getIterador2Final(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo());

			const Periodo periodo_horizonte_estudo_inicial = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, getAtributo(AttComumModeloOtimizacao_estagio_inicial, IdEstagio()), Periodo(), double()).getIteradorInicial();

			struct Estrutura_PTDISPCOMANDADA {
				IdVariavelEstado idVariavelEstado = IdVariavelEstado_Nenhum;
				IdTermeletrica idTermeletrica_importada = IdTermeletrica_Nenhum;
				int codigo_usina = -1;
				IdTermeletrica idTermeletrica = IdTermeletrica_Nenhum;
				IdPatamarCarga idPatamarCarga = IdPatamarCarga_Nenhum;
				Periodo periodo_comandado;
			};

			std::vector<Estrutura_PTDISPCOMANDADA> lista_PTDISPCOMANDADA;
			std::vector<Estrutura_PTDISPCOMANDADA> lista_PTDISPCOMANDADA_ACUMULADO;

			// Validar se estados possuem vari�veis de decis�o no modelo e ajustar cortes se necess�rio.
			for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= estagio.getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

				const std::vector<std::string> nome = estagio.getNomeVariavelEstado(idVariavelEstado);

				//
				// Volume Inicial
				//
				
				if (nome.at(0) == "varZP0_VF_FINF_ACUMULADO") {

					const Periodo periodo(nome.at(2));

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					const int varZP0_VF_FINF_ACUMULANDO = getVarDecisao_ZP0_VF_FINF_ACUMULANDOseExistir(a_TSS, idEstagio, periodo);

					if (varZP0_VF_FINF_ACUMULANDO > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varZP0_VF_FINF_ACUMULANDO);

					else if ((varZP0_VF_FINF_ACUMULANDO == -1) && (a_dados.getAtributo(AttComumDados_mes_penalizacao_volume_util_minimo, IdMes()) == IdMes_Nenhum)) {
						const int varZP0_VF_FINF_ACUMULANDO_nova = addVarDecisao_ZP0_VF_FINF_ACUMULANDO(a_TSS, idEstagio, periodo_estudo_final, 0.0, 0.0, 0.0);
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varZP0_VF_FINF_ACUMULANDO_nova);
					} // else if (varVF == -1)

					else
						throw std::invalid_argument("Nao foi possivel encontrar a variavel varZP0_VF_FINF_ACUMULANDO " + getString(periodo) + " em " + getFullString(idEstagio));

				} // if (nome.at(0) == "varZP0_VF_FINF_ACUMULADO") {
				

				//
				// Volume Inicial
				//

				else if (nome.at(0) == "varVI") {

					const Periodo periodo(nome.at(2));

					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

					if (Periodo(periodo.getTipoPeriodo(), periodo_estudo_final + 1) != periodo)
						throw std::invalid_argument("Periodo estudo invalido " + getFullString(periodo_estudo_final) + " para o acoplamento de varVI de " + getFullString(idVariavelEstado) + " em " + getFullString(periodo));

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					const int varVF = getVarDecisao_VFseExistir(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica);

					if (varVF > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF);

					else if (varVF == -1) {

						const int varVF_nova = addVarDecisao_VF(a_TSS, idEstagio, periodo_estudo_final, idHidreletrica, 0.0, 0.0, 0.0);

						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varVF_nova);

					} // else if (varVF == -1)

				} // if (nome.at(0) == "varVI") {

				//
				// Afluencia Incremental
				//

				else if (nome.at(0) == "varYH") {

					const Periodo periodo(nome.at(2));

					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(3).c_str());

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					const int varYH = criarVariaveis_Decisao_e_Estado_YH_From_CorteBenders(a_TSS, idEstagio, periodo, idHidreletrica);

					if (varYH > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varYH);

					else if (varYH == -1) {
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
					}
				} // else if (nome.at(0) == "varYH") {

				//
				// Afluencia Processo Estocastico
				//

				else if (nome.at(0) == "varYP") {

					const Periodo periodo = Periodo(nome.at(2)) - 1;

					const IdProcessoEstocastico idProcessoEstocastico = getIdProcessoEstocasticoFromChar(nome.at(3).c_str());

					if (idProcessoEstocastico != getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()))
						throw std::invalid_argument( getFullString(idProcessoEstocastico) + " do acoplamento incompativel com " + getFullString(a_dados.processoEstocastico_hidrologico.getAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, IdProcessoEstocastico())) + " do modelo");

					const IdVariavelAleatoria idVariavelAleatoria = getIdVariavelAleatoriaFromChar(nome.at(4).c_str());

					const int lag = getintFromChar(nome.at(5).c_str()) - 1;

					estagio.setVariavelDecisaoEmVariavelEstado(idVariavelEstado, a_TSS, -1);

					Periodo periodo_processo_estocastico_PAR = periodo;

					for (int aux = 0; aux < lag; aux++)
						periodo_processo_estocastico_PAR--;

					IdVariavelEstado idVariavelEstado_criada;

					const int varYP = criarVariaveis_Decisao_e_Estado_YP_ACUMULADO_From_CorteBenders(a_TSS, a_dados, idEstagio, getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_estudo, IdEstagio_1, Periodo()), periodo_processo_estocastico_PAR, lag, idProcessoEstocastico, idVariavelAleatoria, a_horizonte_tendencia_mais_processo_estocastico, idVariavelEstado_criada);

					if (varYP > -1)
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, varYP);

					else if (varYP == -1) {
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
					} // else if (varYH == -1) {

				} // else if (nome.at(0) == "varYP") {


				//
				// Volume viajante
				//

				else if (nome.at(0) == "varVDEF") {

					const Periodo periodo_deslocado(nome.at(3));

					const IdHidreletrica idHidreletrica = getIdHidreletricaFromChar(nome.at(2).c_str());

					if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0) {

						//Cria vari�vel do requerimento do corte e logo cria restri��o com somat�rio as deflu�ncias superpostas dentro do horizonte de aflu�ncias passadas

						const double infinito = vetorEstagio.att(idEstagio).getSolver(a_TSS)->getInfinito();

						const int posVDEF_ACOPLAMENTO = addVarDecisao_VDEF_ACOPLAMENTO(a_TSS, idEstagio, idHidreletrica, periodo_deslocado, -0.0, infinito, 0.0);

						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//Cria restri��o VDEF_ACOPLAMENTO = %1 QDEF1*conversor_vazao_volume_1 + ... + %n QDEFn*conversor_vazao_volume_n
						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						const int posEquDEF_ACOPLAMENTO = addEquLinear_DEFLUENCIA_VIAJANTE_ACOPLAMENTO(a_TSS, idEstagio, idHidreletrica, periodo_deslocado);
						vetorEstagio.att(idEstagio).getSolver(a_TSS)->setRHSRestricao(posEquDEF_ACOPLAMENTO, 0.0);

						vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(posVDEF_ACOPLAMENTO, posEquDEF_ACOPLAMENTO, 1.0);

						//Nota: o horizonte_defluencia_passada come�a no periodo das deflu�ncias passadas e termina no fim do horizonte_estudo
						const Periodo periodo_defluencia_passada_inicial = a_horizonte_defluencia_passada.getIteradorInicial();
						const Periodo periodo_defluencia_passada_final = a_horizonte_defluencia_passada.getIteradorFinal();

						bool sobreposicao_encontrada = false;

						for (Periodo periodo_defluencia_passada = periodo_defluencia_passada_inicial; periodo_defluencia_passada <= periodo_defluencia_passada_final; a_horizonte_defluencia_passada.incrementarIterador(periodo_defluencia_passada)) {

							//O periodo_ficticio_deslocado indica a �gua com tempo de viagem d'�gua que passa para o futuro
							const double sobreposicao = periodo_defluencia_passada.sobreposicao(periodo_deslocado);

							if (sobreposicao > 0.0) {

								const int pos_QDEF = criarVariaveis_Decisao_QDEF_From_CorteBenders(a_TSS, a_dados, idEstagio, periodo_defluencia_passada, periodo_horizonte_estudo_inicial, idHidreletrica);
								
								const double conversor_vazao_volume_periodo = a_dados.getElementoVetor(AttVetorDados_conversor_vazao_volume, periodo_defluencia_passada, double());
								
								vetorEstagio.att(idEstagio).getSolver(a_TSS)->setCofRestricao(pos_QDEF, posEquDEF_ACOPLAMENTO, -sobreposicao* conversor_vazao_volume_periodo);

								sobreposicao_encontrada = true;

							}//if (sobreposicao > 0.0 ) {

							if (sobreposicao == 0.0 && sobreposicao_encontrada) //Se j� tinha encontrado algum periodo com sobreposicao e a sobreposicao volta a ser zero, n�o existem mais periodos com sobreposi��o
								break;

						}//for (Periodo periodo = periodo_inicial; periodo <= periodo_final; a_horizonte_defluencia_passada.incrementarIterador(periodo)) {

						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(idVariavelEstado, a_TSS, posVDEF_ACOPLAMENTO);

					} // if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) > 0) {

					else if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) == 0) {
						estagio.anularVariavelEstadoCorteBenders(idVariavelEstado);
						estagio.vetorVariavelEstado.rem(idVariavelEstado);
					} // else if (a_dados.getAtributo(idHidreletrica, AttComumHidreletrica_tempo_viagem_agua, int()) == 0) {

				} // else if (nome.at(0) == "varVDEF") {

				else if (nome.at(0) == "varPTDISPCOM") {

					Estrutura_PTDISPCOMANDADA estrutura_PTDISPCOMANDADA;

					estrutura_PTDISPCOMANDADA.idVariavelEstado = idVariavelEstado;

					estrutura_PTDISPCOMANDADA.idTermeletrica_importada = getIdTermeletricaFromChar(nome.at(2).c_str());

					estrutura_PTDISPCOMANDADA.codigo_usina = atoi(nome.at(3).c_str());

					IdTermeletrica idTermeletrica = IdTermeletrica_Nenhum;

					for (IdTermeletrica idTermeletrica_aux = IdTermeletrica_1; idTermeletrica_aux <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica_aux++) {
						if (a_dados.getAtributo(idTermeletrica_aux, AttComumTermeletrica_codigo_usina, int()) == estrutura_PTDISPCOMANDADA.codigo_usina) {
							idTermeletrica = idTermeletrica_aux;
							break;
						}
					}

					if (idTermeletrica == IdTermeletrica_Nenhum)
						throw std::invalid_argument("Nao foi possivel criar variavel de estado " + estagio.getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()) + " pois nenhuma termeletrica eh compativel com " + getFullString(estrutura_PTDISPCOMANDADA.idTermeletrica_importada));

					estrutura_PTDISPCOMANDADA.idTermeletrica = idTermeletrica;

					estrutura_PTDISPCOMANDADA.periodo_comandado = Periodo(nome.at(4));

					if (nome.size() == 6)
						estrutura_PTDISPCOMANDADA.idPatamarCarga = getIdPatamarCargaFromChar(nome.at(5).c_str());

					lista_PTDISPCOMANDADA.push_back(estrutura_PTDISPCOMANDADA);

				} // else if (nome.at(0) == "varPTDISPCOM") {

				else if (nome.at(0) == "varPTDISPCOM_ACUMULADO") {

					Estrutura_PTDISPCOMANDADA estrutura_PTDISPCOMANDADA_ACUMULADO;

					estrutura_PTDISPCOMANDADA_ACUMULADO.idVariavelEstado = idVariavelEstado;

					estrutura_PTDISPCOMANDADA_ACUMULADO.idTermeletrica_importada = getIdTermeletricaFromChar(nome.at(2).c_str());

					estrutura_PTDISPCOMANDADA_ACUMULADO.codigo_usina = atoi(nome.at(3).c_str());

					IdTermeletrica idTermeletrica = IdTermeletrica_Nenhum;

					for (IdTermeletrica idTermeletrica_aux = IdTermeletrica_1; idTermeletrica_aux <= a_dados.getMaiorId(IdTermeletrica()); idTermeletrica_aux++) {
						if (a_dados.getAtributo(idTermeletrica_aux, AttComumTermeletrica_codigo_usina, int()) == estrutura_PTDISPCOMANDADA_ACUMULADO.codigo_usina) {
							idTermeletrica = idTermeletrica_aux;
							break;
						}
					}

					if (idTermeletrica == IdTermeletrica_Nenhum)
						throw std::invalid_argument("Nao foi possivel criar variavel de estado " + estagio.getAtributo(idVariavelEstado, AttComumVariavelEstado_nome, std::string()) + " pois nenhuma termeletrica eh compativel com " + getFullString(estrutura_PTDISPCOMANDADA_ACUMULADO.idTermeletrica_importada));

					estrutura_PTDISPCOMANDADA_ACUMULADO.idTermeletrica = idTermeletrica;

					estrutura_PTDISPCOMANDADA_ACUMULADO.periodo_comandado = Periodo(nome.at(4));

					lista_PTDISPCOMANDADA_ACUMULADO.push_back(estrutura_PTDISPCOMANDADA_ACUMULADO);

				} // else if (nome.at(0) == "varPTDISPCOM_ACUMULADO") {

				else
					throw std::invalid_argument("Variavel de estado " + nome.at(0) + " nao compativel com modelo em " + getFullString(idEstagio));  

			} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= estagio.getMaiorId(IdVariavelEstado()); idVariavelEstado++) {

			if (lista_PTDISPCOMANDADA.size() > 0) {

				std::vector<std::vector<int>> indice_PTDISPCOMANDADA(2, std::vector<int>());

				for (int i = 0; i < int(lista_PTDISPCOMANDADA.size()); i++) {
					const int idTermeletrica_int = int(lista_PTDISPCOMANDADA.at(i).idTermeletrica);
					if (indice_PTDISPCOMANDADA.at(0).size() == 0) {
						indice_PTDISPCOMANDADA.at(0).push_back(idTermeletrica_int);
						indice_PTDISPCOMANDADA.at(1).push_back(i);
					}
					else {
						bool termeletrica_indexada = false;
						for (int j = 0; j < indice_PTDISPCOMANDADA.at(0).size(); j++) {
							if (idTermeletrica_int < indice_PTDISPCOMANDADA.at(0).at(j)) {
								indice_PTDISPCOMANDADA.at(0).insert(indice_PTDISPCOMANDADA.at(0).begin() + j, idTermeletrica_int);
								indice_PTDISPCOMANDADA.at(1).insert(indice_PTDISPCOMANDADA.at(1).begin() + j, i);
								termeletrica_indexada = true;
								break;
							}
							else if ((idTermeletrica_int == indice_PTDISPCOMANDADA.at(0).at(j)) && (j < indice_PTDISPCOMANDADA.at(0).size() - 1)) {
								indice_PTDISPCOMANDADA.at(0).insert(indice_PTDISPCOMANDADA.at(0).begin() + j + 1, idTermeletrica_int);
								indice_PTDISPCOMANDADA.at(1).insert(indice_PTDISPCOMANDADA.at(1).begin() + j + 1, i);
								termeletrica_indexada = true;
								break;
							}
						}
						if (!termeletrica_indexada) {
							indice_PTDISPCOMANDADA.at(0).push_back(idTermeletrica_int);
							indice_PTDISPCOMANDADA.at(1).push_back(i);
						}
					}
				} // for (int i = 0; i < int(lista_PTDISPCOMANDADA.size()); i++) {

				for (int i = 0; i < int(lista_PTDISPCOMANDADA.size()); i++) {

					Estrutura_PTDISPCOMANDADA estrutura_PTDISPCOMANDADA = lista_PTDISPCOMANDADA.at(indice_PTDISPCOMANDADA.at(1).at(i));

					estagio.setVariavelDecisaoEmVariavelEstado(estrutura_PTDISPCOMANDADA.idVariavelEstado, a_TSS, -1);

					const int varPTDISPCOM = getVarDecisao_PTDISPCOM(a_TSS, idEstagio, estrutura_PTDISPCOMANDADA.idTermeletrica, estrutura_PTDISPCOMANDADA.codigo_usina, estrutura_PTDISPCOMANDADA.periodo_comandado);

					if (varPTDISPCOM > -1) {
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(estrutura_PTDISPCOMANDADA.idVariavelEstado, a_TSS, varPTDISPCOM);
						criarVariaveis_Decisao_e_Estado_PTDISPCOM_ACUMULADO(a_TSS, a_dados, idEstagio, estrutura_PTDISPCOMANDADA.periodo_comandado, estrutura_PTDISPCOMANDADA.idTermeletrica, estrutura_PTDISPCOMANDADA.codigo_usina);
						//criarRestricoes_PTDISPCOM_CUSTOS(a_dados, idEstagio, estrutura_PTDISPCOMANDADA.periodo_comandado, estrutura_PTDISPCOMANDADA.idTermeletrica, estrutura_PTDISPCOMANDADA.codigo_usina);
					}

					else {
						throw std::invalid_argument("Nao foi possivel encontrar a variavel PTDISPCOM de " + getFullString(estrutura_PTDISPCOMANDADA.idTermeletrica) + " em " + getString(estrutura_PTDISPCOMANDADA.periodo_comandado));
					}

				} // for (int i = 0; i < int(lista_PTDISPCOMANDADA.size()); i++) {

			} // if (lista_PTDISPCOMANDADA.size() > 0) {


			if (lista_PTDISPCOMANDADA_ACUMULADO.size() > 0) {

				std::vector<std::vector<int>> indice_PTDISPCOMANDADA_ACUMULADO(2, std::vector<int>());

				for (int i = 0; i < int(lista_PTDISPCOMANDADA_ACUMULADO.size()); i++) {
					const int idTermeletrica_int = int(lista_PTDISPCOMANDADA_ACUMULADO.at(i).idTermeletrica);
					if (indice_PTDISPCOMANDADA_ACUMULADO.at(0).size() == 0) {
						indice_PTDISPCOMANDADA_ACUMULADO.at(0).push_back(idTermeletrica_int);
						indice_PTDISPCOMANDADA_ACUMULADO.at(1).push_back(i);
					}
					else {
						bool termeletrica_indexada = false;
						for (int j = 0; j < indice_PTDISPCOMANDADA_ACUMULADO.at(0).size(); j++) {
							if (idTermeletrica_int < indice_PTDISPCOMANDADA_ACUMULADO.at(0).at(j)) {
								indice_PTDISPCOMANDADA_ACUMULADO.at(0).insert(indice_PTDISPCOMANDADA_ACUMULADO.at(0).begin() + j, idTermeletrica_int);
								indice_PTDISPCOMANDADA_ACUMULADO.at(1).insert(indice_PTDISPCOMANDADA_ACUMULADO.at(1).begin() + j, i);
								termeletrica_indexada = true;
								break;
							}
							else if ((idTermeletrica_int == indice_PTDISPCOMANDADA_ACUMULADO.at(0).at(j)) && (j < indice_PTDISPCOMANDADA_ACUMULADO.at(0).size() - 1)) {
								indice_PTDISPCOMANDADA_ACUMULADO.at(0).insert(indice_PTDISPCOMANDADA_ACUMULADO.at(0).begin() + j + 1, idTermeletrica_int);
								indice_PTDISPCOMANDADA_ACUMULADO.at(1).insert(indice_PTDISPCOMANDADA_ACUMULADO.at(1).begin() + j + 1, i);
								termeletrica_indexada = true;
								break;
							}
						}
						if (!termeletrica_indexada) {
							indice_PTDISPCOMANDADA_ACUMULADO.at(0).push_back(idTermeletrica_int);
							indice_PTDISPCOMANDADA_ACUMULADO.at(1).push_back(i);
						}
					}
				} // for (int i = 0; i < int(lista_PTDISPCOMANDADA_ACUMULADO.size()); i++) {


				for (int i = 0; i < int(lista_PTDISPCOMANDADA_ACUMULADO.size()); i++) {

					Estrutura_PTDISPCOMANDADA estrutura_PTDISPCOMANDADA_ACUMULADO = lista_PTDISPCOMANDADA_ACUMULADO.at(indice_PTDISPCOMANDADA_ACUMULADO.at(1).at(i));

					estagio.setVariavelDecisaoEmVariavelEstado(estrutura_PTDISPCOMANDADA_ACUMULADO.idVariavelEstado, a_TSS, -1);

					const int varPTDISPCOM_ACUMULADO = criarVariaveis_Decisao_e_Estado_PTDISPCOM_ACUMULADO(a_TSS, a_dados, idEstagio, estrutura_PTDISPCOMANDADA_ACUMULADO.periodo_comandado, estrutura_PTDISPCOMANDADA_ACUMULADO.idTermeletrica, estrutura_PTDISPCOMANDADA_ACUMULADO.codigo_usina);

					if (varPTDISPCOM_ACUMULADO > -1) {
						estagio.setVariavelDecisaoAnteriorEmVariavelEstado(estrutura_PTDISPCOMANDADA_ACUMULADO.idVariavelEstado, a_TSS, varPTDISPCOM_ACUMULADO);

						SmartEnupla<Periodo, double> horizonte_estudo_estagio = getElementosMatriz(AttMatrizModeloOtimizacao_horizonte_estudo, idEstagio, Periodo(), double());

						//for (Periodo periodo_estudo = horizonte_estudo_estagio.getIteradorInicial(); periodo_estudo <= horizonte_estudo_estagio.getIteradorFinal(); horizonte_estudo_estagio.incrementarIterador(periodo_estudo))
							//if (periodo_estudo.sobreposicao(estrutura_PTDISPCOMANDADA_ACUMULADO.periodo_comandado) > 0.0)
								//criarRestricoes_PTDISPCOM_BALANCO_POTENCIA(a_dados, idEstagio, periodo_estudo, estrutura_PTDISPCOMANDADA_ACUMULADO.periodo_comandado, estrutura_PTDISPCOMANDADA_ACUMULADO.idTermeletrica, estrutura_PTDISPCOMANDADA_ACUMULADO.codigo_usina);

					} // if (varPTDISPCOM_ACUMULADO > -1) {

					else {
						estagio.anularVariavelEstadoCorteBenders(estrutura_PTDISPCOMANDADA_ACUMULADO.idVariavelEstado);
						estagio.vetorVariavelEstado.rem(estrutura_PTDISPCOMANDADA_ACUMULADO.idVariavelEstado);
					}

				}

			} // if (lista_PTDISPCOMANDADA_ACUMULADO.size() > 0) {

			vetorEstagio.add(estagio);

		} // if (estagio_final == idEstagio_futuro) {

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::importarVariaveisEstado_AcoplamentoPosEstudo(" + getFullString(a_TSS) + "," + getFullString(a_idProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::importarVariaveisEstado_AcoplamentoPosEstudo(Dados& a_dados, const IdProcesso a_idProcesso, EntradaSaidaDados a_entradaSaidaDados, const SmartEnupla <Periodo, bool> a_horizonte_defluencia_passada) {


void ModeloOtimizacao::exportarVariaveisEstado_AcoplamentoPosEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maior_processo, const IdIteracao a_idIteracao, EntradaSaidaDados a_entradaSaidaDados) {
	try {

		const string diretorio_importacao_pos_estudo = getAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, std::string());

		if (strCompara(diretorio_importacao_pos_estudo, "nenhum"))
			return;

		a_entradaSaidaDados.setDiretorioSaida(a_entradaSaidaDados.getDiretorioSaida() + "//AcoplamentoPosEstudo");

		const IdEstagio idEstagio = getMaiorId(IdEstagio());

		if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial, IdIteracao())) {
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estagio.csv", vetorEstagio.att(idEstagio), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("estado.csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio));
		} // if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_inicial,  IdIteracao())) {

		IdCenario menor_cenario_iteracao = IdCenario_Excedente;
		IdCenario maior_cenario_iteracao = IdCenario_Nenhum;

		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {
			if (getSize1Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor) > 0) {
				if (getSize2Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) {
					const IdCenario menor_cenario = getIterador2Inicial(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario());
					const IdCenario maior_cenario = getIterador2Final(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso, IdCenario());
					if (menor_cenario < menor_cenario_iteracao)
						menor_cenario_iteracao = menor_cenario;
					if (maior_cenario > maior_cenario_iteracao)
						maior_cenario_iteracao = maior_cenario;
				} // if (getSize2Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso) > 0) {
			} // if (getSize1Matriz(idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor) > 0) {
		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maior_processo; idProcesso++) {

		a_entradaSaidaDados.imprimirArquivoCSV_AttMatriz("estado_valores_" + getFullString(a_idIteracao) + ".csv", IdVariavelEstado_Nenhum, vetorEstagio.att(idEstagio), IdProcesso_mestre, a_maior_processo, menor_cenario_iteracao, maior_cenario_iteracao, AttMatrizVariavelEstado_valor);

		if (a_idIteracao == getAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao()))
			a_entradaSaidaDados.imprimirArquivoCSV_AttComum("eop.txt", vetorEstagio.att(getMaiorId(IdEstagio())), std::vector<AttComumEstagio>{AttComumEstagio_idEstagio, AttComumEstagio_periodo_otimizacao});

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::exportarVariaveisEstado_AcoplamentoPosEstudo(a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::exportarEstadosAcoplamentoPreEstudo(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

bool ModeloOtimizacao::isVariavelEstadoInstanciada(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado){
	try{

		return vetorEstagio.att(a_idEstagio).vetorVariavelEstado.isInstanciado(a_idVariavelEstado);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::isVariavelEstadoInstanciada(" + getFullString(a_idEstagio) + "," + getFullString(a_idVariavelEstado) + "): \n" + std::string(erro.what())); }
}



VARIAVEL_DECISAO_2(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
VARIAVEL_DECISAO_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
VARIAVEL_DECISAO_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
VARIAVEL_DECISAO_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
VARIAVEL_DECISAO_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

RESTRICAO_EQUACAO_LINEAR_2(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
RESTRICAO_EQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
RESTRICAO_EQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
RESTRICAO_EQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
RESTRICAO_EQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

RESTRICAO_INEQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
RESTRICAO_INEQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
RESTRICAO_INEQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
RESTRICAO_INEQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
RESTRICAO_INEQUACAO_LINEAR_7(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7);




void ModeloOtimizacao::imprimirSolucaoPorEstagioPorCenario_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//" + a_subdiretorio;

		for (TipoSubproblemaSolver TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); TSS < TipoSubproblemaSolver_Excedente; TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(TSS));

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

				if ((TSS == TipoSubproblemaSolver_viabilidade_hidraulica) && (!getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()))){}
				else {

					VARIAVEL_DECISAO_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
						VARIAVEL_DECISAO_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
						VARIAVEL_DECISAO_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
						VARIAVEL_DECISAO_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
						VARIAVEL_DECISAO_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

				}
			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				RESTRICAO_EQUACAO_LINEAR_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
					RESTRICAO_EQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
					RESTRICAO_EQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
					RESTRICAO_EQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
					RESTRICAO_EQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

					RESTRICAO_INEQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
					RESTRICAO_INEQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
					RESTRICAO_INEQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
					RESTRICAO_INEQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
					RESTRICAO_INEQUACAO_LINEAR_7(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7)
			}

		}

	} // try {

	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolucaoPorEstagioPorCenario(" + getString(a_idProcesso) + "," + a_subdiretorio + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirSolucao(EntradaSaidaDados a_entradaSaidaDados){

VARIAVEL_DECISAO_2(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2)
VARIAVEL_DECISAO_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3)
VARIAVEL_DECISAO_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4)
VARIAVEL_DECISAO_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5)
VARIAVEL_DECISAO_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6)

RESTRICAO_EQUACAO_LINEAR_2(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_2)
RESTRICAO_EQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_3)
RESTRICAO_EQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_4)
RESTRICAO_EQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_5)
RESTRICAO_EQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_6)

RESTRICAO_INEQUACAO_LINEAR_3(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3)
RESTRICAO_INEQUACAO_LINEAR_4(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4)
RESTRICAO_INEQUACAO_LINEAR_5(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5)
RESTRICAO_INEQUACAO_LINEAR_6(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6)
RESTRICAO_INEQUACAO_LINEAR_7(DECLARAR_IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7);


void ModeloOtimizacao::imprimirSolucaoPorEstagioPorCenarioPorRealizacao_porEstagio(const IdProcesso a_idProcesso, const std::string a_subdiretorio, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const std::string diretorio = a_entradaSaidaDados.getDiretorioSaida() + "//" + a_subdiretorio;

		for (TipoSubproblemaSolver TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); TSS < TipoSubproblemaSolver_Excedente; TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(TSS));

			if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) {

				if ((TSS == TipoSubproblemaSolver_viabilidade_hidraulica) && (!getAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, bool()))) {}
				else {

					VARIAVEL_DECISAO_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2)
						VARIAVEL_DECISAO_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3)
						VARIAVEL_DECISAO_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4)
						VARIAVEL_DECISAO_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5)
						VARIAVEL_DECISAO_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6)

				}
			}

			if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

				RESTRICAO_EQUACAO_LINEAR_2(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_2)
					RESTRICAO_EQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_3)
					RESTRICAO_EQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_4)
					RESTRICAO_EQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_5)
					RESTRICAO_EQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_6)

					RESTRICAO_INEQUACAO_LINEAR_3(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3)
					RESTRICAO_INEQUACAO_LINEAR_4(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4)
					RESTRICAO_INEQUACAO_LINEAR_5(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5)
					RESTRICAO_INEQUACAO_LINEAR_6(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6)
					RESTRICAO_INEQUACAO_LINEAR_7(IMPRIMIR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7)
			}

		}

	} // try {

	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::imprimirSolucaoPorEstagioPorCenarioPorRealizacao(" + getString(a_idProcesso) + "," + a_subdiretorio + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::imprimirSolucao(EntradaSaidaDados a_entradaSaidaDados){


bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {
	try {
		const int level = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizarComTratamentoInviabilidade();
		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, level);
		return (level > 0);
	}
	catch (const std::exception& erro) {
		throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what()));
	}
}

bool ModeloOtimizacao::otimizarProblemaComTratamentoNumericoInviabilidade(const TipoSubproblemaSolver a_TSS, const int a_level, const IdEstagio a_idEstagio) {

	try {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS) == nullptr)
			throw std::invalid_argument("Solver invalido (NULO).");

		try {
			if (a_level == 1) {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

			} // if (a_level == 1) {

			else {

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodo(TipoMetodoSolver_primal_simplex);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
			}

		} // try
		catch (const std::exception& erro) {

			const std::string erro_str = std::string(erro.what());

			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {
					setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
					return false;
				}
			}
		}

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, a_level);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {
			if (a_level == 0) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return true;
		}

		const int calculoIIS = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->calcularIIS();

		if (calculoIIS == 1) {
			if (a_level == 0) {
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
			}
			return false;
		}
		else {
			if (a_level > 0)
				return otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_level - 1, a_idEstagio);
			else if (a_level == 0) {
				setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, -1);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();
				return false;
			}
		}

		return false;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizaProblemaComTratamentoNumericoInviabilidade(" + getFullString(a_TSS) + "," + getFullString(a_level) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizaProblemaComTratamentoNumericoInviabilidade(Solver* a_ptrSolver){

void ModeloOtimizacao::setTempoLimiteOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const double a_tempo_limite){

	try {

		if (a_tempo_limite <= 0.0)
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTempoLimite();
		else
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setTempoLimite(a_tempo_limite);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getTempoOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_tempo_limite) + "): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::setTempoLimiteOtimizacao(const IdEstagio a_idEstagio, const double a_tempo_limite){

double ModeloOtimizacao::getTempoOtimizacao(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio) {

	try {

		return vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getTempoOtimizacao();

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getTempoOtimizacao(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getTempoOtimizacao(const IdEstagio a_idEstagio){


double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio) {

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		bool atualizacaoCompletaVariavelRealizacaoInterna = false;
		IdCenario a_idCenario = IdCenario_1;
		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		//
		// Tratamento Num�rico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizarSubproblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario){



double ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio){

	try {

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->setMetodoPadrao();

		resetarVariavelRealizacaoInterna(a_TSS, a_idEstagio);

		const TipoRelaxacaoAfluenciaIncremental tipoRelaxacaoAfluenciaIncremental = getAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental());

		if (tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_truncamento)
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum);

		else if ((tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_truncamento) || (tipoRelaxacaoAfluenciaIncremental == TipoRelaxacaoAfluenciaIncremental_viabilidade_hidraulica_penalizacao))
			atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, TipoSubproblemaSolver_viabilidade_hidraulica, a_idProcesso, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, a_diretorio);

		try{
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
		} // try
		catch (const std::exception& erro) {

			const std::string erro_str = std::string(erro.what());

			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {

					escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

					//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
					otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

					if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) >= 0)
						return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

					else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == -1) {
						//std::cout << "a_idEstagio: " <<  a_idEstagio << "\n";

						setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

						if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

						if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
							setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

						vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();

						return 0.0;

					} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

				} // if (erro_str.find("10005") != std::string::npos) {					
			} // if (erro_str.size() > 5) {

		} // catch (const std::exception & erro) { 

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		//
		// Tratamento Num�rico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::otimizarSubproblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario){


bool ModeloOtimizacao::otimizarProblema(const TipoSubproblemaSolver a_TSS, const IdProcesso a_idProcesso, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio) {

	try {

		bool solucao_proxy = false;

		if (getElementoMatriz(AttMatrizModeloOtimizacao_realizacao_solucao_proxy, a_idEstagio, a_idRealizacao, int()) == 1)
			solucao_proxy = true;

		setElemento(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, 2);

		atualizarModeloOtimizacaoComVariavelRealizacaoInterna(a_TSS, a_idProcesso, a_idEstagio, a_idCenario, a_idRealizacao);

		if (!solucao_proxy)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		try {
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();
		}
		catch (const std::exception& erro) {
			const std::string erro_str = std::string(erro.what());
			if (erro_str.size() > 5) {
				if (erro_str.find("10005") != std::string::npos) {
					escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, erro_str);
					return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, false, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);
				} // if (erro_str.find("10005") != std::string::npos) {
			} // if (erro_str.size() > 5) {
		} // catch (const std::exception & erro) {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		//
		// Tratamento Num�rico de Inviabilidade
		//

		//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
		otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() != TipoStatusSolver_nao_otimalidade)
			return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, solucao_proxy, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);

		const std::string codigo = getString(int(time(0)));
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
		throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblemaLinear(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }


} // double ModeloOtimizacao::otimizarProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao){

void ModeloOtimizacao::escreverLog10005(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_string){

	try{

		const std::string solverStatus = "SolverStatus.txt";

		std::ofstream escritaStream;

		if ((a_idEstagio == IdEstagio_Nenhum) && (a_idCenario == IdCenario_Nenhum) && (a_idRealizacao == IdRealizacao_Nenhum)) {
			escritaStream.open(solverStatus, std::ios_base::out);
			if (!escritaStream.is_open())
				throw std::invalid_argument("Nao foi possivel abrir o arquivo " + solverStatus + ".");
			escritaStream << "";
		}
		else {

			escritaStream.open(solverStatus, std::ios_base::app);
			if (!escritaStream.is_open())
				throw std::invalid_argument("Nao foi possivel abrir o arquivo " + solverStatus + ".");
			escritaStream << getString(a_TSS) << ";" << getString(a_idIteracao) << ";" << getString(a_idEstagio) << ";" << getString(a_idCenario) << ";" << getString(a_idRealizacao) << ";" << a_string << std::endl;

			contadorLog10005++;

			if (contadorLog10005 <= 1) {
				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
			} // if (contadorLog10005 <= 20) {

		}
		escritaStream.close();
		escritaStream.clear();


	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::escreverLog10005(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

}


double ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const std::string a_diretorio) {

	try {

		IdCenario a_idCenario = IdCenario_1;

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

					VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
						VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
						VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
						VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
						VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

						RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
						RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
						RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
						RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
						RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

						RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
						RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
						RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
						RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
						RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7)

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();
				//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << valorObjetivo << "\n";

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				return getCustoTotal(a_idEstagio);

			} // try
			catch (const std::exception & erro) {

				const std::string erro_str = std::string(erro.what());
				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {
						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);
					} // if (erro_str.find("10005") != std::string::npos) {
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()  == TipoStatusSolver_otimalidade) {

		else if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) {
			//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << 0.0 << "\n";

			setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

			return NAN;

		}

		else
			return NAN;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::otimizarProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }


} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const std::string a_diretorio){

double ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio) {

	try {

		if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			try {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool())) {

					VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2);
					VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3);
					VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4);
					VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5);
					VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6);

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool())) {

					RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2);
					RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3);
					RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4);
					RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5);
					RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6);

					RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3);
					RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4);
					RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5);
					RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6);
					RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7);

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();
				//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << valorObjetivo << "\n";

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1) {
					double zf = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())));
					if ((zf > -0.1) && (zf < 0.0))
						zf = 0.0;
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, zf);
				}

				return getCustoTotal(a_idEstagio);

			} // try
			catch (const std::exception & erro) {

				const std::string erro_str = std::string(erro.what());

				std::cout << getFullString(a_idEstagio) << " " << getFullString(a_idCenario) << " " << erro_str << std::endl;

				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {

						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, IdRealizacao_Nenhum, erro_str);

						//otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, 1, a_idEstagio);
						otimizarProblemaComTratamentoNumericoInviabilidade(a_TSS, a_idEstagio);

						if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) >= 0)
							return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_diretorio);

						else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == -1) {
							//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << 0.00 << "\n";
							setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

							if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

							if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
								setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

							vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->resetar();

							return 0.0;

						} // else if (getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int()) == 0) {

					} // if (erro_str.find("10005") != std::string::npos) {					
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getString(getElementoVetor(AttVetorModeloOtimizacao_tratamento_inviabilidade, a_idEstagio, int())) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) { 

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema.");

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getString(a_idEstagio) + "," + getString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio) {


bool ModeloOtimizacao::posOtimizacaoProblema(const TipoSubproblemaSolver a_TSS, const IdIteracao a_idIteracao, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, bool a_solucao_proxy, EstruturaResultados<double>& a_sol_inf_var_dinamica, EstruturaResultados<double>& a_solucao_dual_var_dinamica, EstruturaResultados<double>& a_limite_inferior_var_dinamica, EstruturaResultados<double>& a_limite_superior_var_dinamica, EstruturaResultados<double>& a_sol_dual_var_estado, const std::string a_diretorio) {

	try {

		if ((vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) && (a_solucao_proxy)) {

			try {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, bool())) {

					VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_2)
						VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_3)
						VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_4)
						VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_5)
						VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_VARIAVEL_DECISAO_6)

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, bool())) {

					RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_2)
						RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_3)
						RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_4)
						RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_5)
						RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_EQUACAO_LINEAR_6)

						RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_3)
						RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_4)
						RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_5)
						RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_6)
						RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_POR_REALIZACAO_RESTRICAO_INEQUACAO_LINEAR_7)

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario) + "_" + getString(a_idRealizacao));
				}

				const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorObjetivo();
				//const double valorObjetivo = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getDualObjValue();

				const double valorSolucaoInferior = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getSolucaoInferiorVarDinamica();

				const double valorSolucaoInferiorVarDinamica = valorObjetivo - valorSolucaoInferior;

				a_sol_inf_var_dinamica.vetor[a_sol_inf_var_dinamica.conta] = valorSolucaoInferiorVarDinamica;

				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getReducedCostVarDinamica(a_solucao_dual_var_dinamica.conta, a_solucao_dual_var_dinamica.vetor);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, a_limite_inferior_var_dinamica.vetor);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, a_limite_superior_var_dinamica.vetor);

				getSolucaoDualVariavelEstado(a_idEstagio, a_sol_dual_var_estado.conta, a_sol_dual_var_estado.vetor);
				//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << valorObjetivo << "\n";

				setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, valorObjetivo);

				if (getVarDecisao_ZIseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1)
					setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZI(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()))));

				if (getVarDecisao_ZFseExistir(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())) > -1) {
					double zf = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(getVarDecisao_ZF(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo())));
					if ((zf > -0.1) && (zf < 0.0))
						zf = 0.0;
					setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, zf);
				}

				a_sol_inf_var_dinamica.conta++;
				a_solucao_dual_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_inferior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();
				a_limite_superior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

				a_sol_dual_var_estado.conta += int(getMaiorId(a_idEstagio, IdVariavelEstado()));

				return true;

			} // try
			catch (const std::exception& erro) {

				const std::string erro_str = std::string(erro.what());

				if (erro_str.size() > 5) {
					if (erro_str.find("10005") != std::string::npos) {
						escreverLog10005(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, erro_str);
						return posOtimizacaoProblema(a_TSS, a_idIteracao, a_idEstagio, a_idCenario, a_idRealizacao, false, a_sol_inf_var_dinamica, a_solucao_dual_var_dinamica, a_limite_inferior_var_dinamica, a_limite_superior_var_dinamica, a_sol_dual_var_estado, a_diretorio);
					}
				} // if (erro_str.size() > 5) {

				const std::string codigo = getString(int(time(0)));
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
				throw std::invalid_argument("Codigo identificador do problema linear " + getString(codigo) + " \n" + std::string(erro.what()));

			} // catch (const std::exception & erro) {

		} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

		else if ((vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_tempo_excedido) || (!a_solucao_proxy)) {

			a_sol_inf_var_dinamica.add(0.0);

			for (int i = 0; i < vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica(); i++)
				a_solucao_dual_var_dinamica.add(0.0);

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteInferiorVarDinamica(a_limite_inferior_var_dinamica.conta, a_limite_inferior_var_dinamica.vetor);
			a_limite_inferior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getLimiteSuperiorVarDinamica(a_limite_superior_var_dinamica.conta, a_limite_superior_var_dinamica.vetor);
			a_limite_superior_var_dinamica.conta += vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

			for (int i = 0; i < int(getMaiorId(a_idEstagio, IdVariavelEstado())); i++)
				a_sol_dual_var_estado.add(0.0);
			//std::cout << "a_idEstagio: " <<  a_idEstagio << " valOBJ: " << 0.0 << "\n";

			setElemento(AttVetorModeloOtimizacao_custo_total, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, 0.0);

			setElemento(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, 0.0);

			return false;

		}

		else {
			const std::string codigo = getString(int(time(0)));
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idIteracao) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + getFullString(a_idRealizacao) + "_" + codigo);
			throw std::invalid_argument("O status " + getFullString(vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao()) + " nao retorna solucao do problema. Codigo identificador do problema linear " + getString(codigo));
		}

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::posOtimizacaoProblema(" + getFullString(a_TSS) + "," + getFullString(a_idIteracao) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::posOtimizacaoProblema(const IdEstagio a_idEstagio, const IdCenario a_idCenario, const IdRealizacao a_idRealizacao, const std::string a_diretorio){



void ModeloOtimizacao::calcularCustoPrimalViaSubproblemaMestre(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio, const IdCenario a_idCenario, const std::string a_diretorio, double& a_custo_geral, double& a_custo_individual) {

	try {

		if (vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) {

			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->otimizar();

			if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

				if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, bool())) {

					VARIAVEL_DECISAO_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_2)
						VARIAVEL_DECISAO_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_3)
						VARIAVEL_DECISAO_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_4)
						VARIAVEL_DECISAO_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_5)
						VARIAVEL_DECISAO_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_VARIAVEL_DECISAO_6)

				}

				if (getAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, bool())) {

						RESTRICAO_EQUACAO_LINEAR_2(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_2)
						RESTRICAO_EQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_3)
						RESTRICAO_EQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_4)
						RESTRICAO_EQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_5)
						RESTRICAO_EQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_EQUACAO_LINEAR_6)

						RESTRICAO_INEQUACAO_LINEAR_3(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_3)
						RESTRICAO_INEQUACAO_LINEAR_4(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_4)
						RESTRICAO_INEQUACAO_LINEAR_5(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_5)
						RESTRICAO_INEQUACAO_LINEAR_6(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_6)
						RESTRICAO_INEQUACAO_LINEAR_7(ARMAZENAR_VALOR_POR_ESTAGIO_POR_CENARIO_RESTRICAO_INEQUACAO_LINEAR_7)

				} // if (getAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, bool()) && getAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, bool())) {

				if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {
					criarDiretorio(a_diretorio + "//" + getString(a_TSS));
					vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP(a_diretorio + "//" + getString(a_TSS) + "//PL_" + getString(a_idEstagio) + "_" + getString(a_idCenario));
				} // if ((getAtributo(AttComumModeloOtimizacao_imprimir_solver, bool())) && (getAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, bool()))) {

				IdRealizacao idRealizacao = IdRealizacao_1;

				const Periodo periodo_processo_estocastico = getIterador2Inicial(AttMatrizModeloOtimizacao_horizonte_processo_estocastico_hidrologico, a_idEstagio, Periodo());

				const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

				if (getSize1Matriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0) {
					if (periodo_processo_estocastico >= getIterador2Inicial(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, Periodo()))
						idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo_processo_estocastico, IdRealizacao());
				}

				const int varZT_r = getVarDecisao_ZT(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()), idRealizacao);

				a_custo_individual = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT_r);

				const int varZT = getVarDecisao_ZT(a_TSS, a_idEstagio, getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo()));

				a_custo_geral = vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getValorPrimal(varZT);

				return;

			} // if (vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getStatusOtimizacao() == TipoStatusSolver_otimalidade) {

			const std::string codigo = getString(int(time(0)));
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirLP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirMPS("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->imprimirILP("Erro_" + getString(a_TSS) + "_" + getFullString(a_idEstagio) + "_" + getFullString(a_idCenario) + "_" + codigo);
			throw std::invalid_argument("Erro em otimizar, codigo identificador do problema linear " + codigo);

		} // if (vetorEstagio.att(a_idEstagio).isSolverInstanciado(a_TSS)) {

		return;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::calcularCustoPrimalViaSubproblemaMestre(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::correcaoViabilidadeHidraulica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){



void ModeloOtimizacao::getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio, int& a_idx_sol_dual_var_estado, double* a_sol_dual_var_estado){

	try { 

		const TipoSubproblemaSolver tSS = TipoSubproblemaSolver_geral;

		const IdVariavelEstado maiorIdVariavelEstado = getMaiorId(a_idEstagio, IdVariavelEstado());

		if (maiorIdVariavelEstado == IdVariavelEstado_Nenhum)
			return;

		int idx = a_idx_sol_dual_var_estado;
		for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

			const int idVariavelDecisao = getElementoVetor(a_idEstagio, idVariavelEstado, AttVetorVariavelEstado_idVariavelDecisao, tSS, int());

			const double solucao_dual = vetorEstagio.att(a_idEstagio).getSolver(tSS)->getReducedCost(idVariavelDecisao);

			a_sol_dual_var_estado[idx] = solucao_dual;
			idx++;

		} // for (IdVariavelEstado idVariavelEstado = IdVariavelEstado_1; idVariavelEstado <= maiorIdVariavelEstado; idVariavelEstado++) {

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getSolucaoDualVariavelEstado(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // SmartEnupla<IdVariavelEstado, double> ModeloOtimizacao::getSolucaoDualVariavelEstado(const IdEstagio a_idEstagio){

double* ModeloOtimizacao::getReferenciaValoresEstado(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado, const IdProcesso a_idProcesso, const IdCenario a_idCenario_inicial, const IdCenario a_idCenario_final){
	
	try {	
		return vetorEstagio.att(a_idEstagio).getReferenciaValoresEstado(a_idVariavelEstado, a_idProcesso, a_idCenario_inicial, a_idCenario_final);
	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getReferenciaValoresEstado(" + getString(a_idEstagio) + "," + getString(a_idProcesso) + "," + getString(a_idVariavelEstado) + "," + getString(a_idCenario_inicial) + "," + getString(a_idCenario_final) + "): \n" + std::string(erro.what())); }

} // double* ModeloOtimizacao::getReferenciaValoresEstado(const IdEstagio a_idEstagio, const IdVariavelEstado a_idVariavelEstado, const IdCenario a_idCenario_inicial, const IdCenario a_idCenario_final){


double ModeloOtimizacao::getCustoTotal(const IdEstagio a_idEstagio){

	try{ 
		//std::cout << "a_idEstagio: " <<  a_idEstagio << "\n";
		return getElementoVetor(AttVetorModeloOtimizacao_custo_total, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoTotal(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoTotal(const IdEstagio a_idEstagio){


double ModeloOtimizacao::getCustoImediato(const IdEstagio a_idEstagio){

	try { return getElementoVetor(AttVetorModeloOtimizacao_custo_imediato, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoImediato(" + getString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoPresente(const IdEstagio a_idEstagio){


double ModeloOtimizacao::getCustoFuturo(const IdEstagio a_idEstagio){

	try { return getElementoVetor(AttVetorModeloOtimizacao_custo_futuro, a_idEstagio, double()); } // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCustoFuturo(" + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getCustoFuturo(const IdEstagio a_idEstagio){



double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

	try {

		double probabilidade_abertura = 1.0;

		const IdRealizacao maiorIdRealizacao = getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		if (maiorIdRealizacao > IdRealizacao_1) {

			const Periodo periodo = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			const IdProcessoEstocastico idProcessoEstocastico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

			const IdRealizacao idRealizacao = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, a_idCenario, periodo, IdRealizacao());

			probabilidade_abertura = getElementoMatriz(idProcessoEstocastico, AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, idRealizacao, double());

		}// if (maiorIdRealizacao > IdRealizacao_1) {

		return probabilidade_abertura;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getProbabilidadeAbertura(" + getFullString(a_idEstagio) + "," + getFullString(a_idCenario) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idCenario) {

double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdRealizacao a_idRealizacao) {

	try {

		double probabilidade_abertura = 1.0;

		const IdRealizacao maiorIdRealizacao = getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao());

		if (maiorIdRealizacao > IdRealizacao_1) {

			const Periodo periodo = getAtributo(a_idEstagio, AttComumEstagio_periodo_otimizacao, Periodo());

			probabilidade_abertura = getElementoMatriz(getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()), AttMatrizProcessoEstocastico_probabilidade_realizacao, periodo, a_idRealizacao, double());

		}// if (maiorIdRealizacao > IdRealizacao_1) {

		return probabilidade_abertura;

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getProbabilidadeAbertura(" + getFullString(a_idEstagio) + "," + getFullString(a_idRealizacao) + "): \n" + std::string(erro.what())); }

} // double ModeloOtimizacao::getProbabilidadeAbertura(const IdEstagio a_idEstagio, const IdCenario a_idRealizacao) {


void ModeloOtimizacao::consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){
	try{

		consolidarVariaveis(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);
		consolidarEquacoes(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);
		consolidarInequacoes(a_idProcesso, a_maiorIdProcesso, a_entradaSaidaDados);

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarVariaveis(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }
} // void ModeloOtimizacao::consolidarResultados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

VARIAVEL_DECISAO_2(DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_2)
VARIAVEL_DECISAO_3(DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3)
VARIAVEL_DECISAO_4(DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_4)
VARIAVEL_DECISAO_5(DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_5)
VARIAVEL_DECISAO_6(DECLARAR_CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_6);

void ModeloOtimizacao::consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

	try{

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_variaveis_consolidar = int(lista_variaveis_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_variaveis_consolidar = int(lista_variaveis_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_variaveis_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_variaveis_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_variaveis_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_variaveis_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string variavel_str = lista_variaveis_instanciadas.at(a_TSS).at(i);

				VARIAVEL_DECISAO_2(CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_2)
					VARIAVEL_DECISAO_3(CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_3)
					VARIAVEL_DECISAO_4(CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_4)
					VARIAVEL_DECISAO_5(CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_5)
					VARIAVEL_DECISAO_6(CONSOLIDAR_RESULTADOS_VARIAVEL_DECISAO_6)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception & erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarVariaveis(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarVariaveis(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

RESTRICAO_EQUACAO_LINEAR_2(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_2)
RESTRICAO_EQUACAO_LINEAR_3(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_3)
RESTRICAO_EQUACAO_LINEAR_4(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_4)
RESTRICAO_EQUACAO_LINEAR_5(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_5)
RESTRICAO_EQUACAO_LINEAR_6(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_6);

void ModeloOtimizacao::consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_equacoes_consolidar = int(lista_equacoes_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_equacoes_consolidar = int(lista_equacoes_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_equacoes_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_equacoes_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_equacoes_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_equacoes_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string restricao_str = lista_equacoes_instanciadas.at(a_TSS).at(i);

				    RESTRICAO_EQUACAO_LINEAR_2(CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_2)
					RESTRICAO_EQUACAO_LINEAR_3(CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_3)
					RESTRICAO_EQUACAO_LINEAR_4(CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_4)
					RESTRICAO_EQUACAO_LINEAR_5(CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_5)
					RESTRICAO_EQUACAO_LINEAR_6(CONSOLIDAR_RESULTADOS_RESTRICAO_EQUACAO_LINEAR_6)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarEquacoes(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarEquacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){


RESTRICAO_INEQUACAO_LINEAR_3(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_3)
RESTRICAO_INEQUACAO_LINEAR_4(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_4)
RESTRICAO_INEQUACAO_LINEAR_5(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_5)
RESTRICAO_INEQUACAO_LINEAR_6(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_6)
RESTRICAO_INEQUACAO_LINEAR_7(DECLARAR_CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_7);

void ModeloOtimizacao::consolidarInequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const string diretorio = a_entradaSaidaDados.getDiretorioSaida();

		for (TipoSubproblemaSolver a_TSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); a_TSS < TipoSubproblemaSolver_Excedente; a_TSS++) {

			a_entradaSaidaDados.setDiretorioSaida(diretorio + "//" + getString(a_TSS));

			int numero_inequacoes_consolidar = int(lista_inequacoes_instanciadas.at(a_TSS).size()) / int(a_maiorIdProcesso);

			const int resto_inequacoes_consolidar = int(lista_inequacoes_instanciadas.at(a_TSS).size()) % int(a_maiorIdProcesso);

			int pos_inicial = -1;
			int pos_final = -1;

			pos_inicial = getRank(a_idProcesso) * numero_inequacoes_consolidar;

			if (a_idProcesso < a_maiorIdProcesso) {

				if (numero_inequacoes_consolidar == 0.0)
					return;

				pos_final = getRank(IdProcesso(a_idProcesso + 1)) * numero_inequacoes_consolidar;

			} // if (a_idProcesso < a_maiorIdProcesso) {

			else
				pos_final = int(lista_inequacoes_instanciadas.at(a_TSS).size());

			for (int i = pos_inicial; i < pos_final; i++) {

				const std::string restricao_str = lista_inequacoes_instanciadas.at(a_TSS).at(i);

					RESTRICAO_INEQUACAO_LINEAR_3(CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_3)
					RESTRICAO_INEQUACAO_LINEAR_4(CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_4)
					RESTRICAO_INEQUACAO_LINEAR_5(CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_5)
					RESTRICAO_INEQUACAO_LINEAR_6(CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_6)
					RESTRICAO_INEQUACAO_LINEAR_7(CONSOLIDAR_RESULTADOS_RESTRICAO_INEQUACAO_LINEAR_7)

			} // for (int i = pos_inicial; i < pos_final; i++) {

		} // for (TipoSubproblemaSolver tSS = TipoSubproblemaSolver(TipoSubproblemaSolver_Nenhum + 1); tSS < TipoSubproblemaSolver_Excedente; tSS++){

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::consolidarInequacoes(" + getFullString(a_idProcesso) + "," + getFullString(a_maiorIdProcesso) + ",a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::consolidarIninequacoes(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, EntradaSaidaDados a_entradaSaidaDados){

int ModeloOtimizacao::getMenorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao) {

	try {

		int menor_numero_aberturas = int(getAtributo(a_idEstagio, AttComumEstagio_maiorIdRealizacao, IdRealizacao()));
		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			const IdCenario cenario_inicial = getCenarioInicial(idProcesso, a_idIteracao);
			const IdCenario cenario_final = getCenarioFinal(idProcesso, a_idIteracao);

			const int numero_cenarios = getNumeroCenarios(cenario_inicial, cenario_final);

			if (numero_cenarios > 0) {

				for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

					const IdAbertura abertura_inicial = getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());
					const IdAbertura abertura_final = getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					int numero_aberturas = 0;
					if ((abertura_inicial != IdAbertura_Nenhum) && (abertura_final != IdAbertura_Nenhum))
						numero_aberturas = int(abertura_final - abertura_inicial) + 1;

					if (numero_aberturas < menor_numero_aberturas)
						menor_numero_aberturas = numero_aberturas;

				} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			} // if (numero_cenarios > 0) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		return menor_numero_aberturas;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getMenorNumeroAberturasEstagio(" + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
} // int ModeloOtimizacao::getMenorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio) {

int ModeloOtimizacao::getMaiorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao) {

	try {

		int maior_numero_aberturas = 0;
		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

			const IdCenario cenario_inicial = getCenarioInicial(idProcesso, a_idIteracao);
			const IdCenario cenario_final = getCenarioFinal(idProcesso, a_idIteracao);

			const int numero_cenarios = getNumeroCenarios(cenario_inicial, cenario_final);

			if (numero_cenarios > 0) {

				for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

					const IdAbertura abertura_inicial = getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());
					const IdAbertura abertura_final = getElementoMatriz(a_idIteracao, idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

					int numero_aberturas = 0;
					if ((abertura_inicial != IdAbertura_Nenhum) && (abertura_final != IdAbertura_Nenhum))
						numero_aberturas = int(abertura_final - abertura_inicial) + 1;

					if (numero_aberturas > maior_numero_aberturas)
						maior_numero_aberturas = numero_aberturas;

				} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

			} // if (numero_cenarios > 0) {

		} // for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {

		return maior_numero_aberturas;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getMaiorNumeroAberturasEstagio(" + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
} // int ModeloOtimizacao::getMaiorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio) {

int ModeloOtimizacao::getMenorNumeroAberturasProcessoEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao) {

	try {

		int menor_numero_aberturas = 0;

		const IdCenario cenario_inicial = getCenarioInicial(a_idProcesso, a_idIteracao);
		const IdCenario cenario_final = getCenarioFinal(a_idProcesso, a_idIteracao);

		const int numero_cenarios = getNumeroCenarios(cenario_inicial, cenario_final);

		if (numero_cenarios > 0) {

			for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

				const IdAbertura abertura_inicial = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());
				const IdAbertura abertura_final = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

				int numero_aberturas = 0;
				if ((abertura_inicial != IdAbertura_Nenhum) && (abertura_final != IdAbertura_Nenhum))
					numero_aberturas = int(abertura_final - abertura_inicial) + 1;

				if (numero_aberturas < menor_numero_aberturas)
					menor_numero_aberturas = numero_aberturas;

			} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

		} // if (numero_cenarios > 0) {

		return menor_numero_aberturas;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getMenorNumeroAberturasProcessoEstagio(" + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
} // int ModeloOtimizacao::getMenorNumeroAberturasEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio) {

int ModeloOtimizacao::getMaiorNumeroAberturasProcessoEstagio(const IdProcesso a_idProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao) {

	try {

		int maior_numero_aberturas = 0;

		const IdCenario cenario_inicial = getCenarioInicial(a_idProcesso, a_idIteracao);
		const IdCenario cenario_final = getCenarioFinal(a_idProcesso, a_idIteracao);

		const int numero_cenarios = getNumeroCenarios(cenario_inicial, cenario_final);

		if (numero_cenarios > 0) {

			for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

				const IdAbertura abertura_inicial = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());
				const IdAbertura abertura_final = getElementoMatriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_final_por_cenario_por_estagio, idCenario, a_idEstagio, IdAbertura());

				int numero_aberturas = 0;
				if ((abertura_inicial != IdAbertura_Nenhum) && (abertura_final != IdAbertura_Nenhum))
					numero_aberturas = int(abertura_final - abertura_inicial) + 1;

				if (numero_aberturas > maior_numero_aberturas)
					maior_numero_aberturas = numero_aberturas;

			} // for (IdCenario idCenario = cenario_inicial; idCenario <= cenario_final; idCenario++) {

		} // if (numero_cenarios > 0) {

		return maior_numero_aberturas;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getMaiorNumeroAberturasProcessoEstagio(" + getFullString(a_idProcesso) + "," + getFullString(a_idEstagio) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
} // int ModeloOtimizacao::getMaiorNumeroAberturasProcessoEstagio(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio) {

int ModeloOtimizacao::getNumeroTotalCenariosEmEstados(const IdProcesso a_idProcesso, const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio, const IdIteracao a_idIteracao) {
	try {

		int numero_total_cenarios_em_estados = 0;
		for (IdProcesso idProcesso = IdProcesso_mestre; idProcesso <= a_maiorIdProcesso; idProcesso++) {
			if ((a_idProcesso == idProcesso) || (getElementoVetor(a_idIteracao, AttVetorIteracao_tipo_processamento_paralelo, a_idEstagio, TipoProcessamentoParalelo()) == TipoProcessamentoParalelo_por_abertura))
				numero_total_cenarios_em_estados += getSize2Matriz(a_idEstagio, IdVariavelEstado_1, AttMatrizVariavelEstado_valor, idProcesso);
		}
		
		return numero_total_cenarios_em_estados;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getNumeroTotalCenariosEmEstados(" + getFullString(a_maiorIdProcesso) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }
} // int ModeloOtimizacao::getNumeroTotalCenariosEmEstados(const IdProcesso a_maiorIdProcesso, const IdEstagio a_idEstagio){

int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){

	try {

		return vetorEstagio.att(a_idEstagio).getSolver(a_TSS)->getNumeroVarDinamica();

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getNumeroVariavelDinamica(" + getFullString(a_TSS) + "," + getFullString(a_idEstagio) + "): \n" + std::string(erro.what())); }

} // int ModeloOtimizacao::getNumeroVariavelDinamica(const TipoSubproblemaSolver a_TSS, const IdEstagio a_idEstagio){

IdCenario ModeloOtimizacao::getCenarioInicial(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao){
	try {

		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio) > 0)
			return getIterador1Inicial(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, IdCenario());

		return IdCenario_Nenhum;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCenarioInicial(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }

}

IdCenario ModeloOtimizacao::getCenarioFinal(const IdProcesso a_idProcesso, const IdIteracao a_idIteracao){
	try {

		if (getSize1Matriz(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio) > 0)
			return getIterador1Final(a_idIteracao, a_idProcesso, AttMatrizProcesso_mapeamento_abertura_inicial_por_cenario_por_estagio, IdCenario());

		return IdCenario_Nenhum;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getCenarioFinal(" + getFullString(a_idProcesso) + "," + getFullString(a_idIteracao) + "): \n" + std::string(erro.what())); }
}

int ModeloOtimizacao::getNumeroCenarios(const IdCenario a_cenario_inicial, const IdCenario a_cenario_final) {
	try {

		if ((a_cenario_inicial == IdCenario_Nenhum) || (a_cenario_final == IdCenario_Nenhum))
			return 0;

		return int(a_cenario_final - a_cenario_inicial) + 1;

	} // try
	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::getNumeroCenarios(" + getFullString(a_cenario_inicial) + "," + getFullString(a_cenario_final) + "): \n" + std::string(erro.what())); }
}


void ModeloOtimizacao::criarModeloOtimizacao(Dados &a_dados, EntradaSaidaDados a_entradaSaidaDados) {

	try {

		const IdModeloOtimizacao idModeloOtimizacao = getAtributo(AttComumModeloOtimizacao_idModeloOtimizacao, IdModeloOtimizacao());

		const IdProcesso idProcesso = a_dados.getAtributo(AttComumDados_idProcesso, IdProcesso());

		if ((idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) || (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao)) {

			const IdEstagio estagio_inicial = a_dados.getAtributo(AttComumDados_estagio_inicial, IdEstagio());

			setAtributo(AttComumModeloOtimizacao_estagio_inicial, estagio_inicial);

			const IdEstagio estagio_final = a_dados.getAtributo(AttComumDados_estagio_final, IdEstagio());

			setAtributo(AttComumModeloOtimizacao_estagio_final, estagio_final);
			std::cout << "DEFINIU ATRIBUTOS ESTAGIOS" << "\n";
			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_inicial, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_inicial, Periodo()));
			setAtributo(AttComumModeloOtimizacao_periodo_otimizacao_final, a_dados.getElementoVetor(AttVetorDados_horizonte_otimizacao, estagio_final, Periodo()));

			setAtributo(AttComumModeloOtimizacao_periodo_estudo_inicial, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_inicial, Periodo(), double()).getIteradorInicial());
			setAtributo(AttComumModeloOtimizacao_periodo_estudo_final, a_dados.getElementosMatriz(AttMatrizDados_percentual_duracao_horizonte_estudo, estagio_final, Periodo(), double()).getIteradorFinal());
			std::cout << "DEFINIU ATRIBUTOS HORIZONTES" << "\n";
			if (a_dados.getAtributo(AttComumDados_tipo_estudo, TipoEstudo()) == TipoEstudo_otimizacao_e_simulacao)
				setAtributo(AttComumModeloOtimizacao_realizar_simulacao_final, true);
			else
				setAtributo(AttComumModeloOtimizacao_realizar_simulacao_final, false);

			setAtributo(AttComumModeloOtimizacao_estagio_acoplamento_pre_estudo, a_dados.getAtributo(AttComumDados_estagio_acoplamento_pre_estudo, IdEstagio()));
			setAtributo(AttComumModeloOtimizacao_maior_estagio_impressao_versao_alternativa_cortes, a_dados.getAtributo(AttComumDados_maior_estagio_impressao_versao_alternativa_cortes, IdEstagio()));

			setAtributo(AttComumModeloOtimizacao_foco_numerico, a_dados.getAtributo(AttComumDados_foco_numerico, int()));

			setAtributo(AttComumModeloOtimizacao_multiplicador_tempo_limite_resolucao, a_dados.getAtributo(AttComumDados_multiplicador_tempo_limite_resolucao, double()));

			setAtributo(AttComumModeloOtimizacao_tolerancia_otimalidade, a_dados.getAtributo(AttComumDados_tolerancia_otimalidade, double()));
			setAtributo(AttComumModeloOtimizacao_tolerancia_viabilidade, a_dados.getAtributo(AttComumDados_tolerancia_viabilidade, double()));

			setAtributo(AttComumModeloOtimizacao_numero_maximo_tentativas_resolucao, a_dados.getAtributo(AttComumDados_numero_maximo_tentativas_resolucao, int()));

			setAtributo(AttComumModeloOtimizacao_remover_cortes_dominados, a_dados.getAtributo(AttComumDados_remover_cortes_dominados, bool()));

			setAtributo(AttComumModeloOtimizacao_tipo_aversao_a_risco, a_dados.getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()));
			std::cout << "DEFINIU ATRIBUTOS AVERSAO RISCO" << "\n";
			if (a_dados.getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) == TipoAversaoRisco_CVAR) {
				setVetor(AttVetorModeloOtimizacao_alpha_CVAR,  a_dados.getVetor(AttVetorDados_alpha_CVAR,  IdEstagio(), double()));
				setVetor(AttVetorModeloOtimizacao_lambda_CVAR, a_dados.getVetor(AttVetorDados_lambda_CVAR, IdEstagio(), double()));
			} // if (a_dados.getAtributo(AttComumDados_tipo_aversao_a_risco, TipoAversaoRisco()) == TipoAversaoRisco_CVAR) {

			setAtributo(AttComumModeloOtimizacao_maior_corte_importado,                    a_dados.getAtributo(AttComumDados_maior_corte_importado, IdCorteBenders()));
			setAtributo(AttComumModeloOtimizacao_maior_corte_importado_pos_estudo, a_dados.getAtributo(AttComumDados_maior_corte_importado_pos_estudo, IdCorteBenders()));

			setAtributo(AttComumModeloOtimizacao_tipo_selecao_solucao_proxy, a_dados.getAtributo(AttComumDados_tipo_selecao_solucao_proxy, TipoSelecaoSolucaoProxy()));
			setAtributo(AttComumModeloOtimizacao_solucao_proxy_nula, a_dados.getAtributo(AttComumDados_solucao_proxy_nula, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_solver, a_dados.getAtributo(AttComumDados_imprimir_solver, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_solver_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_imprimir_solver_viabilidade_hidraulica, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_solver_mestre, a_dados.getAtributo(AttComumDados_imprimir_solver_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_tempos_metodo_solucao, a_dados.getAtributo(AttComumDados_imprimir_tempos_metodo_solucao, bool()));

			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_cortes,              a_dados.getAtributo(AttComumDados_diretorio_importacao_cortes, std::string()));

			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_pre_estudo, a_dados.getAtributo(AttComumDados_diretorio_importacao_pre_estudo, std::string()));
			setAtributo(AttComumModeloOtimizacao_diretorio_importacao_pos_estudo, a_dados.getAtributo(AttComumDados_diretorio_importacao_pos_estudo, std::string()));

			setAtributo(AttComumModeloOtimizacao_calcular_custo_primal_via_subproblema_mestre, a_dados.getAtributo(AttComumDados_calcular_custo_primal_via_subproblema_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_calcular_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_calcular_cenario_hidrologico_pre_otimizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_cenario_hidrologico_pre_otimizacao, a_dados.getAtributo(AttComumDados_imprimir_cenario_hidrologico_pre_otimizacao, bool()));
			setAtributo(AttComumModeloOtimizacao_numero_cenarios_tendencia_hidrologica, a_dados.getAtributo(AttComumDados_numero_cenarios_tendencia_hidrologica, int()));
			std::cout << "DEFINIU ATRIBUTOS CENARIOS HIDROLOGICOS" << "\n";
			for (IdIteracao idIteracao = IdIteracao_0; idIteracao <= a_dados.getMaiorId(IdIteracao()); idIteracao++) {
				if (a_dados.vetorIteracao.isInstanciado(idIteracao))
					vetorIteracao.add(a_dados.vetorIteracao.at(idIteracao));
			}

			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_variavel_decisao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario_por_realizacao, bool()));

			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_restricao_por_estagio_por_cenario_por_realizacao, a_dados.getAtributo(AttComumDados_imprimir_resultado_por_estagio_por_cenario_por_realizacao, bool()));
			
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_viabilidade_hidraulica, a_dados.getAtributo(AttComumDados_imprimir_resultado_viabilidade_hidraulica, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_resultado_mestre, a_dados.getAtributo(AttComumDados_imprimir_resultado_mestre, bool()));

			setAtributo(AttComumModeloOtimizacao_tipo_relaxacao_afluencia_incremental, a_dados.getAtributo(AttComumDados_tipo_relaxacao_afluencia_incremental, TipoRelaxacaoAfluenciaIncremental()));

			setAtributo(AttComumModeloOtimizacao_cenario_inicial, a_dados.getAtributo(AttComumDados_menor_cenario_do_processo, IdCenario()));
			setAtributo(AttComumModeloOtimizacao_cenario_final, a_dados.getAtributo(AttComumDados_maior_cenario_do_processo, IdCenario()));

			setAtributo(AttComumModeloOtimizacao_numero_cenarios, a_dados.getAtributo(AttComumDados_numero_cenarios, int()));

			setAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, a_dados.getAtributo(AttComumDados_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico()));

			setAtributo(AttComumModeloOtimizacao_imprimir_estado_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_estado_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_corte_por_iteracao, bool()));
			setAtributo(AttComumModeloOtimizacao_imprimir_selecao_corte_por_iteracao, a_dados.getAtributo(AttComumDados_imprimir_selecao_corte_por_iteracao, bool()));
			std::cout << "DEFINIU ATRIBUTOS SELECAO CORTE" << "\n";
			if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) {

				setAtributo(AttComumModeloOtimizacao_iteracao_inicial,  a_dados.getAtributo(AttComumDados_iteracao_inicial, IdIteracao()));
				setAtributo(AttComumModeloOtimizacao_iteracao_final,  IdIteracao(a_dados.getAtributo(AttComumDados_numero_maximo_iteracoes, int()) + 1));

				setAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, a_dados.getAtributo(AttComumDados_selecao_cortes_nivel_dominancia, int()));
				std::cout << "DEFINIU ATRIBUTOS ITERACAO INI, FIM" << "\n";
			} // if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) {

			else if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao) { 

				setAtributo(AttComumModeloOtimizacao_iteracao_final, IdIteracao_0);

				setAtributo(AttComumModeloOtimizacao_selecao_cortes_nivel_dominancia, 0);
				std::cout << "DEFINIU ATRIBUTOS CORTES DOMINANCIA" << "\n";

			} // else if (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao) {
			std::cout << "INSTANCIA PROCESSO ESTOCASTICO" << "\n";
			instanciarProcessoEstocastico(a_dados, a_entradaSaidaDados, a_dados.getAtributo(AttComumDados_idProcesso, IdProcesso()));
			std::cout << "FORMULA MODELO OTIMIZACAO" << "\n";
			formularModeloOtimizacao(a_dados, a_entradaSaidaDados);
			std::cout << "FORMULOU MODELO OTIMIZACAO" << "\n";
		} // if ((idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_otimizacao) || (idModeloOtimizacao == IdModeloOtimizacao_multiestagio_estocastico_simulacao)) {

		else
			throw std::invalid_argument("ModeloOtimizacao invalido");

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::criarModeloOtimizacao(): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::criarModeloOtimizacao(){


void ModeloOtimizacao::instanciarProcessoEstocastico(Dados& a_dados, EntradaSaidaDados a_entradaSaidaDados, const IdProcesso a_idProcesso) {

	try{

		a_entradaSaidaDados.setDiretorioEntrada(a_entradaSaidaDados.getDiretorioEntrada() + "//ProcessoEstocasticoHidrologico");

		const IdProcessoEstocastico tipo_processo_estocastico_hidrologico = getAtributo(AttComumModeloOtimizacao_tipo_processo_estocastico_hidrologico, IdProcessoEstocastico());

		ProcessoEstocastico processo_estocastico_hidrologico;
		processo_estocastico_hidrologico.setAtributo(AttComumProcessoEstocastico_idProcessoEstocastico, tipo_processo_estocastico_hidrologico);

		vetorProcessoEstocastico.add(processo_estocastico_hidrologico);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("PROCESSO_ESTOCASTICO_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_probabilidade_realizacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttVetorProcessoEstocastico_mapeamento_tendencia_temporal) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_direto);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_residuo_espaco_amostral) + ".csv",            vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);
		
		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_" + getString(AttMatrizVariavelAleatoria_coeficiente_linear_auto_correlacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membro);

		a_entradaSaidaDados.carregarArquivoCSV_AttComum_seExistir("VARIAVEL_ALEATORIA_INTERNA_AttComumOperacional.csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_grau_liberdade) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);
		a_entradaSaidaDados.carregarArquivoCSV_AttVetor_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttVetorVariavelAleatoriaInterna_coeficiente_participacao) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("VARIAVEL_ALEATORIA_INTERNA_" + getString(AttMatrizVariavelAleatoriaInterna_tendencia_temporal) + ".csv", vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico), TipoAcessoInstancia_membroMembro);

		ProcessoEstocastico processoEstocastico;

		const IdCenario idCenarioInicial = getAtributo(AttComumModeloOtimizacao_cenario_inicial, IdCenario());
		const IdCenario idCenarioFinal = getAtributo(AttComumModeloOtimizacao_cenario_final, IdCenario());

		a_entradaSaidaDados.carregarArquivoCSV_AttMatriz_seExistir("PROCESSO_ESTOCASTICO_" + getString(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) + ".csv", processoEstocastico, TipoAcessoInstancia_direto);

		if (processoEstocastico.getSizeMatriz(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral) > 0)
			vetorProcessoEstocastico.att(tipo_processo_estocastico_hidrologico).setCenariosMapeamento(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, processoEstocastico.getCenariosMapeamento(AttMatrizProcessoEstocastico_mapeamento_espaco_amostral, idCenarioInicial, idCenarioFinal));

	} // try

	catch (const std::exception& erro) { throw std::invalid_argument("ModeloOtimizacao(" + getString(getIdObjeto()) + ")::instanciarProcessoEstocastico(a_entradaSaidaDados): \n" + std::string(erro.what())); }

} // void ModeloOtimizacao::instanciarProcessoEstocastico(EntradaSaidaDados a_entradaSaidaDados) {
