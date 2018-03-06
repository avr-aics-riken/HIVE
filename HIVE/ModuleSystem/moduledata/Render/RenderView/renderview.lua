
RenderView = {}
setmetatable(RenderView, {__index = HiveBaseModule})

local JSON = require('dkjson')

RenderView.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.cam = Camera()    
    this.property = {
        screensize = {512, 512},
        position = {0,0,300},
        target = {0,0,0},
        up = {0,1,0},
        fov = 60,
        clearcolor = {0,0,0,1},
        color_file = "",
        depth_file = "",
        ipcmode = false
    }
    this.network_ipc = nil
    this.gen = PrimitiveGenerator()
    
    setmetatable(this, {__index=RenderView})
    return this
end

function insertLineVertex(vlist, v1, v2)
    for k, v in ipairs(v1) do
        table.insert(vlist, v);
    end
    for k, v in ipairs(v2) do
        table.insert(vlist, v);
    end
end

function transform(v, trans)
	return {
        trans[1] * v[1] + trans[5] * v[2] + trans[9]  * v[3] + trans[13] * 1.0,
        trans[2] * v[1] + trans[6] * v[2] + trans[10] * v[3] + trans[14] * 1.0,
        trans[3] * v[1] + trans[7] * v[2] + trans[11] * v[3] + trans[15] * 1.0,
        --trans[4] * v[1] + trans[8] * v[2] + trans[12] * v[3] + trans[16] * 1.0
    };
end

function screenSpaceLineVertex(cam, vlist, v1, v2)
    table.insert(vlist, cam:ConvertToScreenSpace(v1[1], v1[2], v1[3]));
    table.insert(vlist, cam:ConvertToScreenSpace(v2[1], v2[2], v2[3]));
end

function getBBoxVerts(bmin, bmax)
    local res = {}
    local p1 = { bmin[1], bmax[2], bmin[3] };
    local p2 = { bmax[1], bmax[2], bmin[3] };
    local p3 = { bmax[1], bmin[2], bmin[3] };
    local p4 = { bmax[1], bmin[2], bmax[3] };
    local p5 = { bmin[1], bmin[2], bmax[3] };
    local p6 = { bmin[1], bmax[2], bmax[3] };
    table.insert(res, p1);
    table.insert(res, p2);
    table.insert(res, p3);
    table.insert(res, p4);
    table.insert(res, p5);
    table.insert(res, p6);
    table.insert(res, bmin);
    table.insert(res, bmax);
    return res;
end

function getBBoxMinMax(verts)
    local bmin = {
        math.min(verts[1][1], verts[2][1], verts[3][1], verts[4][1], verts[5][1], verts[6][1], verts[7][1], verts[8][1]),
        math.min(verts[1][2], verts[2][2], verts[3][2], verts[4][2], verts[5][2], verts[6][2], verts[7][2], verts[8][2]),
        math.min(verts[1][3], verts[2][3], verts[3][3], verts[4][3], verts[5][3], verts[6][3], verts[7][3], verts[8][3]),
    };
    local bmax = {
        math.max(verts[1][1], verts[2][1], verts[3][1], verts[4][1], verts[5][1], verts[6][1], verts[7][1], verts[8][1]),
        math.max(verts[1][2], verts[2][2], verts[3][2], verts[4][2], verts[5][2], verts[6][2], verts[7][2], verts[8][2]),
        math.max(verts[1][3], verts[2][3], verts[3][3], verts[4][3], verts[5][3], verts[6][3], verts[7][3], verts[8][3]),
    };
    return {bmin, bmax}
end

function RenderView:Do()
    self:UpdateValue()
    local v = self.value
    
    self.cam:SetScreenSize(v.rendersize[1], v.rendersize[2])--v.screensize[1], v.screensize[2])
    self.cam:SetFilename(v.color_file)
    self.cam:SetDepthFilename(v.depth_file)
    self.cam:ClearColor(v.clearcolor[1],v.clearcolor[2],v.clearcolor[3],v.clearcolor[4])
    self.cam:LookAt(
        v.position[1], v.position[2], v.position[3],
        v.target[1],   v.target[2],   v.target[3],
        v.up[1],       v.up[2],       v.up[3],
        v.fov
    )
    
    local temp = {}
    local targetcam
-- For Object
    if v.RenderObject then
        temp = v.RenderObject
    end
-- For bbox
    if v.Camera then
        targetcam = v.Camera
    else
        targetcam = self.cam
    end
    
    local bbox_verts2D = {}
    if (v.showbbox == true and v.RenderObject) then
        local line_verts = {}
        local shadername = '../shader/const.frag';
        for i, t in ipairs(v.RenderObject) do
            local bbox = t:GetBBox();
            if bbox[1] ~= nil and bbox[2] ~= nil then
                local verts = getBBoxVerts(bbox[1], bbox[2]);
                local trans = t:GetTransformMatrix();
                local bverts = {};
                for k, v in ipairs(verts) do
                    table.insert(bverts, transform(v, trans));
                end
                --local bboxTransed = getBBoxMinMax(bverts);
                --bverts = getBBoxVerts(bboxTransed[1], bboxTransed[2]);
                insertLineVertex(line_verts, bverts[7], bverts[1]);
                insertLineVertex(line_verts, bverts[1], bverts[2]);
                insertLineVertex(line_verts, bverts[2], bverts[3]);
                insertLineVertex(line_verts, bverts[3], bverts[7]);
                insertLineVertex(line_verts, bverts[7], bverts[5]);
                insertLineVertex(line_verts, bverts[1], bverts[6]);
                insertLineVertex(line_verts, bverts[2], bverts[8]);
                insertLineVertex(line_verts, bverts[3], bverts[4]);
                insertLineVertex(line_verts, bverts[8], bverts[4]);
                insertLineVertex(line_verts, bverts[4], bverts[5]);
                insertLineVertex(line_verts, bverts[5], bverts[6]);
                insertLineVertex(line_verts, bverts[6], bverts[8]);
            end
        end
        local line = self.gen:LineList(line_verts, #line_verts / 3, 1.0);
        local linemodel = LineModel();
        linemodel:Create(line)
        linemodel:SetShader(shadername)
        linemodel:SetVec4("color", v.bboxcolor[1], v.bboxcolor[2], v.bboxcolor[3], v.bboxcolor[4]);
        linemodel:SetLineWidth(1)
        temp[#temp + 1] = linemodel
    end
        
-- For Camera
    if v.Camera then
        temp[#temp + 1] = v.Camera
        targetcam = v.Camera
    else
        temp[#temp + 1] = self.cam
        targetcam = self.cam
    end

    if v.floatbuffer then
        targetcam:SetImageBufferFormat('RGBA32F')
    else
        targetcam:SetImageBufferFormat('RGBA8')
    end
-- dump
--[[
    for i,v in pairs(temp) do
         print('RenderObject->', v);   
    end
--]]
--[[
    if v.screensize[1] / 10 < v.rendersize[1] then
        render(temp, HIVE_fetchEvent)
    else
    ]]
        render(temp)
    --end
    
    
    if network == nil and HIVE_metabin == nil then -- no UI mode        
        return true
    end 
    
    local mode = 'png'
    -- image save
    local imageBuffer
    local imageBufferSize
    if self.property.ipcmode then
        mode = 'raw'
        -- image save
        local img = targetcam:GetImageBuffer()	
        imageBuffer = img:GetBuffer()
        imageBufferSize = img:GetSize()
    else    
        -- image save
        imageBuffer = HIVE_ImageSaver:SaveMemory(5, targetcam:GetImageBuffer())
        imageBufferSize = HIVE_ImageSaver:MemorySize()
    end
    
    -- create metabinary
    --local w = v.screensize[1]
    --local h = v.screensize[2]
    local w = v.rendersize[1]
    local h = v.rendersize[2]
    print('rendersize=('.. w ..",".. h ..")", 'cancel=', tostring(HIVE_isRenderCanceled))
    
    if targetClientId == nil then
        return false
    end
    
    local colorbar_rgba = "";
    local colorbar_min = "0";
    local colorbar_max = "0";
    local colobar_color = "[1, 1, 1, 1]";
    local colobar_composite = "false";
    if v.ColorBar then
        colorbar_rgba = JSON.encode(v.ColorBar.rgba);
        colorbar_min = tostring(v.ColorBar.min)
        colorbar_max = tostring(v.ColorBar.max)
        if v.ColorBar.color ~= nil then
            colobar_color = JSON.encode(v.ColorBar.color);
        end
        if v.ColorBar.composite ~= nil then
            colobar_composite = tostring(v.ColorBar.composite);
        end
    end
    
    local json = [[{
            "JSONRPC" : "2.0",
            "method" : "renderedImage",            
            "to" : ]] .. targetClientId ..[[,
            "param" : {
                "type" : "]] .. mode .. [[",
                "width" : "]] .. w .. [[",
                "height" : "]] .. h .. [[",
                "canceled": ]] .. tostring(HIVE_isRenderCanceled) .. [[,
                "varname": "]] .. self.varname .. [[",
                "colorbar": {
                    "rgba": "]] .. colorbar_rgba .. [[",
                    "min": "]] .. colorbar_min .. [[",
                    "max": "]] .. colorbar_max .. [[",
                    "color" : "]] .. colobar_color .. [[",
                    "composite" : "]] .. colobar_composite .. [["
                },
                "bbox" : "]] ..JSON.encode(bbox_verts2D) .. [["
            },
            "id":0
    }]]
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    --print('JSON=', json, 'size=', imageBufferSize)
    -- send
    print('ipcmode', self.property.ipcmode)        
    if self.property.ipcmode then       
        if self.network_ipc == nil then
            local ipcAddress = 'ipc:///tmp/HIVE_IPC_' .. self.varname
            print('IPC open=', ipcAddress);
	        self.network_ipc = LoadModule("Network").Connection()
	        local ipcr = self.network_ipc:Connect(ipcAddress)
	        print('IPC ret=', ipcr)
        end
        print('IPC->', self.network_ipc)
        self.network_ipc:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('IPC SEND!!!!!!!!!!!')			   

    else
        network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
        print('WEBSOCKET SEND!!!!!!!!!!!')			   

    end
    return true
end

function RenderView:Camera()
    return self.cam
end

function RenderView:Image()
    return self.cam:GetImageBuffer()
end

function RenderView:ScreenSize()
    return self.value.screensize
end