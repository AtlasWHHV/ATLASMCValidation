#!/bin/env bash
#
# Run the validation script
#

# Setup in a special athena directory
if [ ! -d athena ]; then
  mkdir athena
fi
cd athena
export MCJO_LOCATION=../MCValidation/mcjo
export MCSCRIPTS_LOCATION=../MCValidation/scripts

# Now run the thing
echo Working on run $MCRUN
make -e -f ../MCValidation/scripts/Makefile $MCRUN

cd ..
export MCJO_LOCATION=MCValidation/mcjo
export MCSCRIPTS_LOCATION=MCValidation/scripts
source ~gwatts/bin/CommonScripts/configASetup.sh
rcSetup 2.2.5,Base
make -e -f $MCSCRIPTS_LOCATION/Makefile.plots $MCRUN
