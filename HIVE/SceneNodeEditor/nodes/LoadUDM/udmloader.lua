

function LoadUDM(property)
	local udm = UDMLoader()
	udm:Load(property.filepath)
	return udm
end
