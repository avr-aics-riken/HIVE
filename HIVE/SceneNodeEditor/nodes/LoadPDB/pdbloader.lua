function LoadPDB(property)
	local pdbloader
	pdbloader = require("PdbLoader")()
	pdbloader:Load(property.filepath)
	return pdbloader
end
