###############################################################
# Pythia8 H_v -> pi_v pi_v
# contact: Gordon Watts (gwatts@uw.edu)
#===============================================================
evgenConfig.description = "Higgs (mH300) to pi_v (mVPI100) to displaced heavy fermions"
evgenConfig.keywords = ["exotic",  "hiddenValley", "longLived"]
evgenConfig.contact = ["gwatts@uw.edu"]

# Specify MSTW2008LO PDF
include("MC12JobOptions/Pythia8_AU2_MSTW2008LO_Common.py")

topAlg.Pythia8.Commands += ["ParticleDecays:limitTau0 = off"] # Allow long-lived particles to decay
 
topAlg.Pythia8.Commands += ["35:name = H_v"]       # Set H_v name
topAlg.Pythia8.Commands += ["36:name = pi_v"]      # Set pi_v name

topAlg.Pythia8.Commands += ["Higgs:useBSM = on"]   # Turn on BSM Higgses
topAlg.Pythia8.Commands += ["HiggsBSM:gg2H2 = on"] # Turn on gg->H_v production

topAlg.Pythia8.Commands += ["35:onMode = off"]     # Turn off all H_v decays
topAlg.Pythia8.Commands += ["35:onIfAll = 36 36"]  # Turn back on H_v->pi_vpi_v

topAlg.Pythia8.Commands += ["35:m0 = 600"]         # Set H_v mass

topAlg.Pythia8.Commands += ["36:m0 = 150"]          # Set pi_v mass
topAlg.Pythia8.Commands += ["36:tau0 = 1330"]       # Set pi_v lifetime

# Turn off checks for displaced vertices. Include the filter
# to make sure it is appended and we can access it (the code
# won't include it again).
include("EvgenJobTransforms/Generate_TestHepMC.py")
topAlg.TestHepMC.MaxVtxDisp = 1000*1000 #In mm
topAlg.TestHepMC.MaxTransVtxDisp = 1000*1000
