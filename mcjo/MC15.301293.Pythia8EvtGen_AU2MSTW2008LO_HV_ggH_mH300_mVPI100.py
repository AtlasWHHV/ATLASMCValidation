###############################################################
# Pythia8 H_v -> pi_v pi_v
# contact: Gordon Watts (gwatts@uw.edu)
#===============================================================
evgenConfig.description = "Higgs (mH300) to pi_v (mVPI100) to displaced heavy fermions"
evgenConfig.keywords = ["exotic",  "hiddenValley", "longLived"]
evgenConfig.contact = ["gwatts@uw.edu"]

# Specify MSTW2008LO PDF
include("MC15JobOptions/Pythia8_A2_MSTW2008LO_EvtGen_Common.py")

genSeq.Pythia8.Commands += ["ParticleDecays:limitTau0 = off"] # Allow long-lived particles to decay
 
genSeq.Pythia8.Commands += ["35:name = H_v"]       # Set H_v name
genSeq.Pythia8.Commands += ["36:name = pi_v"]      # Set pi_v name

genSeq.Pythia8.Commands += ["Higgs:useBSM = on"]   # Turn on BSM Higgses
genSeq.Pythia8.Commands += ["HiggsBSM:gg2H2 = on"] # Turn on gg->H_v production

genSeq.Pythia8.Commands += ["35:onMode = off"]     # Turn off all H_v decays
genSeq.Pythia8.Commands += ["35:onIfAll = 36 36"]  # Turn back on H_v->pi_vpi_v

genSeq.Pythia8.Commands += ["35:m0 = 300"]         # Set H_v mass

genSeq.Pythia8.Commands += ["36:m0 = 100"]          # Set pi_v mass
genSeq.Pythia8.Commands += ["36:tau0 = 1330"]       # Set pi_v lifetime

# Turn off checks for displaced vertices. Other checks are fine.
testSeq.TestHepMC.MaxVtxDisp = 1000*1000 #In mm
testSeq.TestHepMC.MaxTransVtxDisp = 1000*1000
