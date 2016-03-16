import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';

export default class Container extends React.Component {
    constructor(props) {
        super(props);

        this.store = props.store;
        this.action = props.action;
		let n = this.props.node;
		n.uiComponent = eval(this.props.node.uiFunc);
        this.state = {
            node: this.props.node,
            closeHover: false,
			containerHover : null // or zindex
        };
        this.isLeftDown = false;
        this.isScaleLeftDown = false;

        this.nodeChanged = this.nodeChanged.bind(this);
        this.componentDidMount = this.componentDidMount.bind(this);
        this.componentWillUnmount = this.componentWillUnmount.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
        this.onScaleMove = this.onScaleMove.bind(this);
        this.onScaleUp = this.onScaleUp.bind(this);
        this.onScaleDown = this.onScaleDown.bind(this);
        this.onCancelBubble = this.onCancelBubble.bind(this);
    }

    nodeChanged(err, data) {
        if(data.varname === this.state.node.varname){
            this.setState({node: Object.assign({}, data)});
        }
    }

    componentDidMount() {
        window.addEventListener('mousemove', this.onMouseMove);
        window.addEventListener('mouseup', this.onMouseUp);
        window.addEventListener('mousemove', this.onScaleMove);
        window.addEventListener('mouseup', this.onScaleUp);
        this.store.on(Core.Constants.NODE_INPUT_CHANGED, this.nodeChanged);
        this.store.on(Core.Constants.PANEL_CHANGED, this.nodeChanged);
        this.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.nodeChanged);
    }

    componentWillUnmount() {
        window.removeEventListener('mousemove', this.onMouseMove);
        window.removeEventListener('mouseup', this.onMouseUp);
        window.removeEventListener('mousemove', this.onScaleMove);
        window.removeEventListener('mouseup', this.onScaleUp);
        this.store.removeListener(Core.Constants.NODE_INPUT_CHANGED, this.nodeChanged);
        this.store.removeListener(Core.Constants.PANEL_CHANGED, this.nodeChanged);
        this.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.nodeChanged);
    }

    onMouseDown(ev) {
        ev.stopPropagation();
        if(ev.button === 0){
            this.isLeftDown = true;
			if (this.props.zoom) {
				this.mousePos = {x: ev.clientX  * this.props.zoom - this.state.node.panel.pos[0], y: ev.clientY  * this.props.zoom- this.state.node.panel.pos[1]};
			} else {
				this.mousePos = {x: ev.clientX - this.state.node.panel.pos[0], y: ev.clientY - this.state.node.panel.pos[1]};
			}
			this.selectNode(this.state.node);
            this.forwardIndex(this.state.node);
        }
    }

    onMouseUp(ev) {
        this.isLeftDown = false;
    }

    onMouseMove(ev) {
        if (this.isLeftDown) {
            let node = this.state.node;
            let mv;
            let panel = JSON.parse(JSON.stringify(node.panel));
			if (this.props.zoom) {
			 	mv = {x: ev.clientX * this.props.zoom - this.mousePos.x, y: ev.clientY  * this.props.zoom- this.mousePos.y};
				panel.pos[0] = mv.x;
				panel.pos[1] = mv.y;
			} else {
			 	mv = {x: ev.clientX - this.mousePos.x, y: ev.clientY - this.mousePos.y};
	            panel.pos[0] = mv.x;
	            panel.pos[1] = mv.y;
			}
            this.action.changeNode({
                varname : node.varname,
                panel : panel
            });
        }
        ev.preventDefault();
    }

    onScaleDown(ev) {
        ev.stopPropagation();
        if(ev.button === 0){
            this.isScaleLeftDown = true;
            this.scalePos = {x: ev.clientX, y: ev.clientY};
            this.offsetScaleLeft = Math.max(ev.currentTarget.offsetLeft + ev.currentTarget.offsetWidth, 100);
            this.offsetScaleTop  = Math.max(ev.currentTarget.offsetTop + ev.currentTarget.offsetHeight, 100);
			this.selectNode(this.state.node);
            this.forwardIndex(this.state.node);
        }
    }

    onScaleUp(ev) {
        this.isScaleLeftDown = false;
    }

    onScaleMove(ev) {
        if (this.isScaleLeftDown) {
            let node = this.state.node;
            let mv = {x: ev.clientX - this.scalePos.x, y: ev.clientY - this.scalePos.y};
            let panel = JSON.parse(JSON.stringify(node.panel));
            panel.size[0] = Math.max(this.offsetScaleLeft + mv.x, 100);
            panel.size[1] = Math.max(this.offsetScaleTop + mv.y, 100);
            this.action.changeNode({
                varname : node.varname,
                panel : panel
            });
        }
    }

    onCancelBubble(ev){
        ev.stopPropagation();
    }

    // 閉じるボタンが押された.
    onCloseClick(ev) {
        this.action.changePanelVisible(this.state.node.varname, false);
    }

    // 閉じるボタンにマウスホバーされた
    onCloseHover(ev) {
        this.setState({ closeHover : !this.state.closeHover });
    }

	onContainerEnter(ev) {
		let panel = JSON.parse(JSON.stringify(this.state.node.panel));
		let preZIndex = panel.zindex;
		panel.zindex = 10000;
		this.action.changeNode({
			varname : this.state.node.varname,
			panel : panel
		});
		this.setState({ containerHover : preZIndex });
	}

	onContainerLeave(ev) {
		let panel = JSON.parse(JSON.stringify(this.state.node.panel));
		panel.zindex = this.state.containerHover;
		this.action.changeNode({
			varname : this.state.node.varname,
			panel : panel
		});
		this.setState({ containerHover : null });
	}

	selectNode(target) {
		this.action.unSelectNode([]);
		let nodes = this.store.getNodes();
		let panel = JSON.parse(JSON.stringify(this.state.node.panel));
		panel.zindex = nodes.length;
		this.action.changeNode({
			varname : target.varname,
			select : true,
			panel : panel
		});
	}

    // index を最前面に持ってくる
    // target === ターゲットノード
    forwardIndex(target) {
		let nodes = this.store.getNodes();
		let targetIndex = this.state.containerHover;
		if (targetIndex === null) { return; }
		for (let i = 0; i < nodes.length; i = i + 1) {
			let node = nodes[i];
			if (node.varname !== target.varname && node.panel.zindex >= targetIndex){
				let panel = JSON.parse(JSON.stringify(node.panel));
				panel.zindex = targetIndex - 1;
				this.action.changeNode({
					varname : node.varname,
					panel : panel
				});
			}
		}
    }

    styles() {
        return {
            container : {
                backgroundColor: "rgb(69, 69, 69)",
                border: "2px solid " + (this.state.node.select ? "rgba(33, 187, 151, 1.0)" : "rgba(0, 0, 0, 0.0)"),
                borderRadius: "5px",
                color: "whitesmoke",
                margin : "0px",
                padding : "0px",
                minWidth : this.state.node.panel.size[0] + "px",
                minHeight: this.state.node.panel.size[1] + "px",
                position: "absolute",
                top:  this.state.node.panel.pos[1] + "px",
                left: this.state.node.panel.pos[0] + "px",
                zIndex: this.state.node.panel.zindex,
                display: this.state.node.panel.visible ? "block" : "none",
				transform : (this.state.containerHover !== null && this.props.zoom) ? "scale(" + this.props.zoom + ")": ""
            },
            panelTitleBar: {
                fontSize: "12pt",
                lineHeight: "30px",
                minHeight: "30px",
                margin: "0px",
                padding: "0px 5px",
                cursor: "move",
                textShadow: "0px 0px 3px black"
            },
            panelCloseButton: {
                backgroundColor : "rgb(33, 187, 151)",
                borderRadius : "5px",
                color: "white",
                margin : "6px",
                width: "18px",
                height: "18px",
                position : "absolute",
                top: "0px",
                right: "0px",
                opacity: this.state.closeHover ? "0.9" : "1.0",
                backgroundImage : "url(./img/node_close.png)",
                backgroundRepeat: "no-repeat",
                backgroundSize: "contain",
                cursor: "pointer"
            },
            panelScale: {
                backgroundColor: "rgb(33, 187, 151)",
                position: "absolute",
                bottom: "0px",
                right: "0px",
                width: "18px",
                height: "18px",
                overflow: "hidden",
                cursor: "se-resize"
            }
        };
    }

	uiComponent() {
		let node = this.state.node;
		if (node.uiComponent === undefined) {
			return;
		} else { // if (node.jsx === true) {
			let UIComponent = this.state.node.uiComponent;
			return (<UIComponent
						store={this.store}
						action={this.action}
						node={node}
					/>);
		}
	}

    render() {
        var node, styles = this.styles();
        return (
            <div ref="bounds" style={styles.container}  onMouseDown={this.onMouseDown}
				onMouseEnter={this.onContainerEnter.bind(this)}
				onMouseLeave={this.onContainerLeave.bind(this)}
				>
                <div style={styles.panelTitleBar}>
                    {this.state.node.name}
                    <div
                        style={styles.panelCloseButton}
                        onClick={this.onCloseClick.bind(this)}
                        onMouseEnter={this.onCloseHover.bind(this)}
                        onMouseLeave={this.onCloseHover.bind(this)}
                    />
                </div>
                <div onMouseDown={this.onCancelBubble}>
					{this.uiComponent.bind(this)()}
				</div>
                <div style={styles.panelScale} onMouseDown={this.onScaleDown.bind(this)}>
                </div>
            </div>
        );
    }
}
