function PolygonAnalizeFunc(arg)
	local model = PolygonModel()
	model:Create(arg.mesh)
	print('PolygonAnalyzerFunc')
 	local analy = require("Analyzer").PolygonAnalyzer()
	analy:Execute(model)
	return {
    	min=function () return {analy:MinX(), analy:MinY(), analy:MinZ()} end,
    	max=function () return {analy:MaxX(), analy:MaxY(), analy:MaxZ()} end
	}
end
