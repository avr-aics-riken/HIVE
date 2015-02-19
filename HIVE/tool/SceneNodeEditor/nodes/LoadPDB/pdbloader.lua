function LoadPDB(property)
	local pdbloader
	pdbloader = PDBLoader()
	pdbloader:Load(property.filepath)
	return pdbloader
end
