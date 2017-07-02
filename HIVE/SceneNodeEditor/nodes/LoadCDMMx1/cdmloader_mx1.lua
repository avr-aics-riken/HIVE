
function LoadCDMMx1(property)
   local loader = require("CdmLoader")()
   loader:LoadMx1(property.filepath)
   return loader
end
