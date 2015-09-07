

function LoadUDM(property)
	local udm = UDMLoader()
	udm:Load(property.filepath)

	local udminst = {
		Tetra = function()
			return udm:TetraData()
		end,
		Mesh = function ()
			return udm:MeshData()
		end,
		ExtraP = function ()
			return udm:ExtraData('P')
		end,
		ExtraT = function ()
			return udm:ExtraData('TEMPARATURE')
		end,
		ExtraU = function ()
			return udm:ExtraData('U')
		end,
		ExtraV = function ()
			return udm:ExtraData('V')
		end,
		ExtraW = function ()
			return udm:ExtraData('W')
		end
	}
	return udminst
end
