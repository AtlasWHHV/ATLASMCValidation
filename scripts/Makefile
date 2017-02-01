# Make the xAOD and the EVNT files
# source files.
#
# Meant to be run from the directory where MCValidation is
# checked out to with the command
# make -f MCValidation/scripts/Makefile
# Use "-n" to see what commands would be issued (standard Makefile way of
# running).
#

# Common default definitions. Often reset in a "production" environment.
MCJO_LOCATION = MCValidation/mcjo
MCSCRIPTS_LOCATION = MCValidation/scripts
MCRESULT = $(PWD)
NEVENTS = 200
MCTYPE = MC12

# Make everything
all: 191300

191300: $(MCRESULT)/191300.$(MCTYPE).xAOD.pool.root
191301: $(MCRESULT)/191301.$(MCTYPE).xAOD.pool.root

# Add a tag (like -pythia) if we want to have generation by different code.
# But in general, if you can't use Generate_trf, then you can't use the
# ATLAS production system!

# Make sure we don't get tripped up, keep all files generated
# along the way, and make sure all doesn't require a temp file.
.PHONY: all

.SECONDARY:

# Common commands to get us running easily (without much work above).

# Generate the evnt file for a particular run.
$(MCRESULT)/%.$(MCTYPE).EVNT.pool.root: $(MCJO_LOCATION)/$(MCTYPE).%.*.py
ifeq ($(MCTYPE),MC15)
	source ~gwatts/bin/CommonScripts/configASetup.sh; source $$AtlasSetup/scripts/asetup.sh 19.2.3.5,here,AtlasProduction; export JOBOPTSEARCHPATH=/cvmfs/atlas.cern.ch/repo/sw/Generators/MC15JobOptions/latest/common:$${JOBOPTSEARCHPATH}; Generate_tf.py --runNumber $* --ecmEnergy 13000.0 --firstEvent 1 --maxEvents $(NEVENTS) --jobConfig $< --outputEVNTFile $@ | tee $@.log
	cp log.generate $@.generate.log
endif
ifeq ($(MCTYPE),MC14)
	source ~gwatts/bin/CommonScripts/configASetup.sh; source $$AtlasSetup/scripts/asetup.sh 19.2.3.4,here,AtlasProduction; Generate_tf.py --runNumber $* --ecmEnergy 13000.0 --firstEvent 1 --maxEvents $(NEVENTS) --jobConfig $< --outputEVNTFile $@ | tee $@.log
	cp log.generate $@.generate.log
endif
ifeq ($(MCTYPE),MC12)
	source ~gwatts/bin/CommonScripts/configASetup.sh; source $$AtlasSetup/scripts/asetup.sh 17.2.13.9,here; export JOBOPTSEARCHPATH=/cvmfs/atlas.cern.ch/repo/sw/Generators/MC12JobOptions/latest/common:$${JOBOPTSEARCHPATH}; Generate_trf.py runNumber=$* ecmEnergy=13000.0 firstEvent=1 maxEvents=$(NEVENTS) jobConfig=$< outputEVNTFile=$@  randomSeed=1234 | tee $@.log
	cp Generate.log $@.generate.log
endif



$(MCRESULT)/%.$(MCTYPE).xAOD.pool.root: $(MCRESULT)/%.$(MCTYPE).EVNT.pool.root
	source ~gwatts/bin/CommonScripts/configASetup.sh; source $$AtlasSetup/scripts/asetup.sh 20.1.X.Y-VAL,rel_1,AtlasDerivation,gcc48,here; mkdir -p InstallArea/jobOptions/DerivationFrameworkMCTruth; cp $(MCSCRIPTS_LOCATION)/TRUTH1.py InstallArea/jobOptions/DerivationFrameworkMCTruth; Reco_tf.py --inputEVNTFile $< --outputDAODFile xAOD.pool.root --reductionConf TRUTH1 | tee $@.log
	#source ~gwatts/bin/CommonScripts/configASetup.sh; source $$AtlasSetup/scripts/asetup.sh 20.1.X.Y-VAL,rel_2,AtlasDerivation,gcc48,here; Reco_tf.py --inputEVNTFile $< --outputDAODFile xAOD.pool.root --reductionConf TRUTH1 | tee $@.log
	mv DAOD_TRUTH1.xAOD.pool.root $@
