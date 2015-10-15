all: ep2

ep2:
	echo '#!/bin/bash' > ep2
	echo 'export PYTHONPATH="src/simulator"' >> ep2
	echo 'python3 src/shell/main.py "$$@"' >> ep2
	chmod +x ep2

clean:
	rm -f ep2
