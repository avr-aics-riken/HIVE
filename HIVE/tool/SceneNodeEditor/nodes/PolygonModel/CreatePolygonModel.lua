function CreatePolygonModel(property)
	local pm = PolygonModel();
	pm:Create(property.mesh);
	pm:SetShader(property.shadername)
	function getModel()
		return pm
	end
	return {
		model=getModel
	}
end
