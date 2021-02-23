#include "QQbarProcessor.hh"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCGenericObjectImpl.h"
#include "IMPL/LCCollectionVec.h"
#include <marlin/ProcessorMgr.h> 
#include "marlin/Exceptions.h"

using namespace lcio ;
using namespace marlin ;
using std::vector;
using std::string;
using std::abs;
using EVENT::Track;
using IMPL::ReconstructedParticleImpl;
using IMPL::MCParticleImpl;
using EVENT::ParticleID;
using IMPL::ParticleIDImpl;

namespace QQbarProcessor
{
  QQbarProcessor aQQbarProcessor ;


  QQbarProcessor::QQbarProcessor() : Processor("QQbarProcessor") 
  {
	  
    // modify processor description
    
    // register steering parameters: name, description, class-variable, default value
    registerProcessorParameter( "ROOTFileName",
				"Output ROOT File Name",
				_hfilename,
				string("QQbarProcessor.root") );
    registerProcessorParameter( "AnalysisType",
				"Analysis Type (0= default qqbar -started by A. Irles)",
				_type,
				_type );
    registerProcessorParameter( "DBDanalysis",
				"It is a DBD analysis?",
				_boolDBDanalysis,
				true
				);
    registerProcessorParameter( "newPandoraPFO",
                                "Has a PandoraPFO collection been duplicarted? i.e. for dEdx reprocessinn?",
				_newPandoraPFO,
                                false
                                );
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE , 
			     "PFOCollection",
			     "PFO collection name"  ,
			     _colName ,
			     string("PandoraPFOs")
			     ) ;
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE ,
                             "newPFOCollection",
                             "new PFO collection name, only used in the case of some highlevel preprocessing as dEdx Likelihood",
                             _newcolName ,
                             string("NewPandoraPFOs")
                             ) ;
    registerInputCollection( LCIO::LCRELATION,
			    "Old2NewPandoraPFOsLink" ,
			     "link between old and new Pandora PFO (only used if _newPandoraPFO==true" ,
			     _Old2NewPandoraPFOsLink,
			    std::string("Old2NewPandoraPFOsLink")
			    );
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			     "JetCollectionName",
                             "Name of the Jet collection",
			     _JetsColName,
			     std::string("FinalJets")
                             );

    registerInputCollection( LCIO::LCRELATION,
			     "JetRelCollectionName" , 
			     "Name of the PrimaryVertex collection"  ,
			     _JetsRelColName ,
			     std::string("FinalJets_rel")
			     );
    registerInputCollection( LCIO::MCPARTICLE,
			     "MCCollectionName" , 
			     "Name of the MC collection"  ,
			     _MCColName ,
			     std::string("MCParticles")
			     );
    registerInputCollection( LCIO::LCRELATION,
			     "RecoMcTruthCollectionName" , 
			     "Name of the RecoMcTruth collection"  ,
			     _colRelName ,
			     std::string("RecoMCTruthLink")
			     );
    registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			     "initialJetCollectionName" , 
			     "Name of the initialJet collection, only used for DBD analysis"  ,
			     _initialJetsColName ,
			     std::string("InitialJets")
			     );
    
    registerProcessorParameter( "Rparam_jet_ps",
				"R parameter of the ee_gentkt algorithm (for parton + shower analysis)",
				_Rparam_jet_ps,
				(float)1.5 );  
    registerProcessorParameter( "pparam_jet_ps",
				"p parameter of the ee_gentkt algorithm (for parton + shower analysis)",
				_pparam_jet_ps,
				(float)1.0 );
  }
	
  void QQbarProcessor::init() 
  { 
    // usually a good idea to

    std::cout << "Type: " <<  _type << "\n";
    _analysisType = static_cast<ANALYSIS_TYPE>(_type);
    std::cout << "Type: " <<  _analysisType << "\n";

    printParameters() ;
    std::cout << "_analysisType: " << _analysisType << "\n";
    _nRun = 0 ;

    // Initialize sum
    switch(_analysisType)
      {
      case QQbar:
	std::cout << "Initialize QQbarTree, _analysisType= " << _analysisType << "\n";
	_qqbaranalysis.Init(_hfilename);
	break;
      }

  }
  void QQbarProcessor::processRunHeader( LCRunHeader* run) 
  { 
    _nRun++ ;
  } 

  void QQbarProcessor::processEvent( LCEvent * evt )
  {
    switch(_analysisType)
      {
      case QQbar: 
	{ 
	  _qqbaranalysis.AnalyseQQbar(evt,
				      _boolDBDanalysis,
				      _newPandoraPFO,
				      _colName ,
				      _newcolName ,
				      _colRelName,
				      _initialJetsColName,
				      _JetsColName ,
				      _JetsRelColName ,
				      _MCColName,
				      _Old2NewPandoraPFOsLink,
				      _Rparam_jet_ps,
				      _pparam_jet_ps
				      );
	}
	break;
       	break;
      }
  }

  void QQbarProcessor::check( LCEvent * evt ) 
  {
  }


  void QQbarProcessor::end()
  {   

    switch(_analysisType)
      {

      case QQbar:
	_qqbaranalysis.End();
	break;

      }
  }
  
}
