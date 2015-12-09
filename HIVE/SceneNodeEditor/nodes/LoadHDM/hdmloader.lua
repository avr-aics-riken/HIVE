

function LoadHDM(property)
	local hdm = require("HdmLoader")()
	hdm:Init(property.cellidpath, property.datapath)
	-- field name, field type, components(1=scalar, 3=vector), stepNo
	local volumedata = hdm:LoadField(property.fieldname, property.fieldtype, 1, 0)
	function sparsevolume()
		return volumedata
	end
	return {sparsevolume=sparsevolume}
end
