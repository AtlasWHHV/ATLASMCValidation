#==============================================================================
# Set up common services and job object.
# This should appear in ALL derivation job options
#==============================================================================
from DerivationFrameworkCore.DerivationFrameworkMaster import *

#====================================================================
# ATTACH THE RECONSTRUCTION TO THE SEQUENCER  
#====================================================================

# Add translator from EVGEN input to xAOD-like truth here
from RecExConfig.ObjKeyStore import objKeyStore
from xAODTruthCnv.xAODTruthCnvConf import xAODMaker__xAODTruthCnvAlg
if objKeyStore.isInInput( "McEventCollection", "GEN_EVENT" ):
    DerivationFrameworkJob += xAODMaker__xAODTruthCnvAlg("GEN_EVNT2xAOD",AODContainerName="GEN_EVENT")

#====================================================================
# JET/MET
#====================================================================
# Set jet flags
from JetRec.JetRecFlags import jetFlags
jetFlags.useTruth = True
jetFlags.useTracks = False
# Add jet algorithms
from JetRec.JetAlgorithm import addJetRecoToAlgSequence
addJetRecoToAlgSequence(DerivationFrameworkJob,eventShapeTools=None)
from JetRec.JetRecStandard import jtm
from JetRec.JetRecConf import JetAlgorithm
jetFlags.truthFlavorTags = ["BHadronsInitial", "BHadronsFinal", "BQuarksFinal",
                            "CHadronsInitial", "CHadronsFinal", "CQuarksFinal",
                            "TausFinal",
                            "Partons",
                            ]
# Standard truth jets
# To recover jet constituents remove the last modifier.
akt4 = jtm.addJetFinder("AntiKt4TruthJets", "AntiKt", 0.4, "truth", modifiersin=[jtm.truthpartondr, jtm.partontruthlabel, jtm.removeconstit], ptmin= 5000)
akt4alg = JetAlgorithm("jetalgAntiKt4TruthJets", Tools = [akt4] )
DerivationFrameworkJob += akt4alg

# WZ Truth Jets
#jtm.addJetFinder("AntiKt4TruthWZJets",  "AntiKt", 0.4,  "truthwz", ptmin= 5000)
#jtm.addJetFinder("AntiKt6TruthWZJets",  "AntiKt", 0.6,  "truthwz", ptmin= 5000)
# Other jets
#akt6  = jtm.addJetFinder("AntiKt6TruthJets", "AntiKt", 0.6, "truth", ptmin= 5000)
#akt10 = jtm.addJetFinder("AntiKt10TruthJets", "AntiKt", 1.0, "truth", ptmin= 5000)
#akt10trim = jtm.addJetTrimmer("TrimmedAntiKt10TruthJets", rclus=0.3, ptfrac=0.05, input='AntiKt10TruthJets')

# Add truth-based MET algorithm here
import METReconstruction.METConfig_Truth
from METReconstruction.METRecoFlags import metFlags # not sure if you even need this line
from METReconstruction.METRecoConfig import getMETRecoAlg
metAlg = getMETRecoAlg('METReconstruction')
DerivationFrameworkJob += metAlg

from DerivationFrameworkMCTruth.TruthObjectTools import *
from DerivationFrameworkMCTruth.TruthDecoratorTools import *



#==============================================================================
# Thinning the master truth collection 
#==============================================================================
from DerivationFrameworkMCTruth.DerivationFrameworkMCTruthConf import DerivationFramework__MenuTruthThinning
TRUTH1TruthThinning = DerivationFramework__MenuTruthThinning(name                       = "TRUTH1TruthThinning",
                                                            ThinningService            = "TRUTH1ThinningSvc",
                                                            WritePartons               = False,
                                                            WriteHadrons               = False,
                                                            WriteBHadrons              = False,
                                                            WriteGeant                 = False,
                                                            GeantPhotonPtThresh        = -1.0,
                                                            WriteTauHad                = True,
                                                            PartonPtThresh             = -1.0,
                                                            WriteBSM                   = True,
                                                            WriteBosons                = True,
                                                            WriteBSMProducts           = True,
                                                            WriteBosonProducts         = True,
                                                            WriteTopAndDecays          = True,
                                                            WriteEverything            = False,
                                                            WriteAllLeptons            = False,
                                                            WriteStatus3               = False,
                                                            PreserveDescendants        = False, 
                                                            PreserveGeneratorDescendants = False,
                                                            PreserveAncestors          = False,
                                                            WriteFirstN                = 10)
ToolSvc += TRUTH1TruthThinning

#==============================================================================
# Create the derivation kernel algorithm
#==============================================================================
from DerivationFrameworkCore.DerivationFrameworkCoreConf import DerivationFramework__DerivationKernel
DerivationFrameworkJob += CfgMgr.DerivationFramework__DerivationKernel("TRUTH1Kernel",
                                                                        AugmentationTools = [TRUTH1MuonTool,TRUTH1ElectronTool,TRUTH1PhotonTool,TRUTH1TauTool,
                                                                          TRUTH1ElectronDressingTool, TRUTH1MuonDressingTool,
                                                                          TRUTH1ElectronIsolationTool1, TRUTH1ElectronIsolationTool2,
                                                                          TRUTH1MuonIsolationTool1, TRUTH1MuonIsolationTool2,
                                                                          TRUTH1PhotonIsolationTool1, TRUTH1PhotonIsolationTool2],
)

#==============================================================================
# Set up stream
#==============================================================================
streamName = derivationFlags.WriteDAOD_TRUTH1Stream.StreamName
fileName = buildFileName( derivationFlags.WriteDAOD_TRUTH1Stream )
TRUTH1Stream = MSMgr.NewPoolRootStream( streamName, fileName )
# Thinning
from AthenaServices.Configurables import ThinningSvc, createThinningSvc
augStream = MSMgr.GetStream( streamName )
evtStream = augStream.GetEventStream()
svcMgr += createThinningSvc( svcName="TRUTH1ThinningSvc", outStreams=[evtStream] )

# Only events that pass the filters listed are written out
# AcceptAlgs  = logical OR of filters
# RequireAlgs = logical AND of filters
TRUTH1Stream.AcceptAlgs(['TRUTH1Kernel'])

#==============================================================================
# Set up slimming content list here
#==============================================================================
TRUTH1Stream.AddItem("xAOD::EventInfo#*")
TRUTH1Stream.AddItem("xAOD::EventAuxInfo#*")
TRUTH1Stream.AddItem("xAOD::JetContainer#*")
TRUTH1Stream.AddItem("xAOD::JetAuxContainer#*")
TRUTH1Stream.AddItem("xAOD::MissingETContainer#*")
TRUTH1Stream.AddItem("xAOD::MissingETAuxContainer#*")

TRUTH1Stream.AddItem( "xAOD::TruthEventContainer#*" )
TRUTH1Stream.AddItem( "xAOD::TruthEventAuxContainer#*" )
TRUTH1Stream.AddItem( "xAOD::TruthVertexContainer#*" )
TRUTH1Stream.AddItem( "xAOD::TruthVertexAuxContainer#*" )
TRUTH1Stream.AddItem( "xAOD::TruthParticleContainer#*" )
TRUTH1Stream.AddItem( "xAOD::TruthParticleAuxContainer#*" )

