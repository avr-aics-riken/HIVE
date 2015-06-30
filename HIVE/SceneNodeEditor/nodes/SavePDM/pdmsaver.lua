function SavePDM(property)
	local saver = PDMSaver()
	saver:SetPointData(property.pointdata)
	saver:Save(property.basefilepath)
end
