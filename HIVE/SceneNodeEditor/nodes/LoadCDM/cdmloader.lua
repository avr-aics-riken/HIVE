
function LoadCDM(property)
   local loader = require("CdmLoader")()
   if property.migration >= 1 then
      loader:LoadMxN(property.filepath,
                     property.division[1],
                     property.division[2],
                     property.division[3])
   else
      loader:LoadMxM(property.filepath)
   end
   return loader
end
