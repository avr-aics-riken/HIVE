

function LoadUDM(property)
	local udm = UDMLoader()
	return udm:Load(property.filepath)
end
