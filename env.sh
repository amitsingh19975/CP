# Define CPT_SCRIPT_DIR in your environment.

export PATH="$PATH:$CPT_SCRIPT_DIR/bin"

# making cpt excutable
chmod +x $CPT_SCRIPT_DIR/bin/cpt

if [ ! -d "$CPT_SCRIPT_DIR/bin/env" ]; then
    python3 -m venv $CPT_SCRIPT_DIR/bin/env 
fi

if [ ! -f "$CPT_SCRIPT_DIR/bin/env/bin/activate" ]; then
    echo "Error: $CPT_SCRIPT_DIR/bin/env/bin/activate not found."
    exit 1
fi

source $CPT_SCRIPT_DIR/bin/env/bin/activate

