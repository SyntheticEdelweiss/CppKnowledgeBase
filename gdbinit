python
import sys
sys.path.insert(0, sys.path[0] + '/../../gcc-14.1.0/python')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end

# Skip all headers
#skip -gfi C:/msys64/mingw64/include/*.h