

module.exports = {
    INITIALIZED : "initialized",
    NODE_CHANGED : "node_changed",
    NODE_INPUT_CHANGED : "node_input_changed",
	NODE_POSITION_CHANGED : "node_position_changed",
	NODE_CLOSE_CHANGED : "node_close_changed",
    PLUG_CHANGED : "plug_changed",
    NODE_COUNT_CHANGED : "node_count_changed",
    PLUG_COUNT_CHANGED : "plug_count_changed",
    IMAGE_RECIEVED : "image_revieved",
    RENDERER_LOG_RECIEVED: "renderer_log_recieved",
    ANALYZED_DATA_RECIEVED: "analyzed_data_recieved",
    NODE_ADDED : "node_added",
    NODE_DELETED : "node_deleted",
    NODE_SELECTE_CHANGED : "node_selected",
    PLUG_ADDED : "plug_added",
    PLUG_DELETED : "plug_deleted",
	PANEL_CHANGED : "panel_changed",
	PANEL_VISIBLE_CHANGED : "panel_visible_changed",
	PANEL_SIZE_CHANGED : "panel_size_changed",
	COPY_CALLED : "copy_called",
	PASTE_CALLED : "paste_called",
	DELETE_CALLED : "delete_called",
	MAKE_GROUP_CALLED : "make_group_called",
    LAYOUT_CHANGED : "layout_changed",
    CONSOLEOUTPUT_SHOW: "consoleoutput_show",
	CHANGE_FRAME : "change_frame",
	KEYFRAME_ADDED : "keyframe_added"
};

// color 定数化したい気もする
// backgroundColor :  this.state.isSelected ? "rgba(2, 17, 27, 0.6)" : "rgba(67, 67, 67, 0.9)",
// borderColor : this.state.isSelected ? "rgb(54, 196, 168)" : "rgba(0, 0, 0, 0)"
