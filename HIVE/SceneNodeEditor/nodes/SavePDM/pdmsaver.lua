function SavePDM(property)
	local saver = require("PdmSaver")()()
	saver:SetPointData(property.pointdata)
	saver:Save(property.basefilepath)
end
