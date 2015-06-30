
function LoadCDM(property)
	local loader = CDMLoader()
	loader:Load(property.filepath)
	return loader
end
