import React from "react";
import ReactDOM from "react-dom";
import Core from '../../Core';

export default class MenuNodeList extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            nodeList: this.props.store.getNodeNameList()
        };

        this.styles = this.styles.bind(this);
        this.floating = (this.props.floating !== undefined) && this.props.floating;
        this.unFocus = this.unFocus.bind(this);
        this.props.store.on(Core.Constants.INITIALIZED, (()=>{
            this.setState({nodeList: this.props.store.getNodeNameList()});
        }).bind(this));
		this.onClick = this.onClick.bind(this);
		this.draggingElem = null;
    }

    onClick(value){
		return (eve) => {
	        this.props.action.addNodeByName(value);
	        this.unFocus();
	        if(this.props.hasOwnProperty("hiddenFunction")){this.props.hiddenFunction();}
		}
    }

    unFocus(){
        var e = ReactDOM.findDOMNode(this.refs.select);
        e.selectedIndex = -1;
    }

    styles() {
        if(this.floating){
            return {
                block: {
                    // backgroundColor: "rgba(0, 0, 0, 0.7)",
                    textAlign: "center",
                    margin: "0px",
                    padding: "0px",
                    width: "248px",
                    height: "218px",
                    overflow: "auto"
                },
                select: {
                    backgroundColor: "transparent",
                    border: "none",
                    color: "rgb(2, 255, 245)",
                    fontSize: "large",
                    margin: "0px auto",
                    padding: "0px",
                    width: "240px",
                    minHeight: "100%",
                    overflow: "visible"
                },
                list: {
                    // backgroundColor: "rgba(128, 128, 128, 0.5)",
                    backgroundColor: "rgb(83, 83, 83)",
                    borderRadius: "3px",
                    color: "rgb(2, 255, 245)",
                    fontSize: "small",
                    textAlign: "left",
                    lineHeight: "24px",
                    margin: "3px 2px",
                    padding: "2px",
                    width: "236px",
                    height: "24px",
                    overflow: "hidden",
                    textShadow: "0px 0px 3px #022",
                    boxShadow: "0px 0px 5px -2px white",
                    cursor: "pointer"
                }
            };
        }else{
            return {
                block: {
                    margin: "0px",
                    padding: "0px",
                    width: "100%",
                    height: "248px",
                    overflow: "auto"
                },
                select: {
                    backgroundColor: "transparent",
                    border: "none",
                    color: "rgb(2, 255, 245)",
                    fontSize: "large",
                    margin: "0px",
                    padding: "0px",
                    height: "100%",
                    overflow: "visible"
                },
                list: {
                    // backgroundColor: "rgba(128, 128, 128, 0.5)",
                    backgroundColor: "rgb(83, 83, 83)",
                    borderRadius: "3px",
                    color: "rgb(2, 255, 245)",
                    fontSize: "12px",
                    lineHeight: "20px",
                    margin: "3px",
                    padding: "2px",
                    width: "250px",
                    height: "20px",
                    overflow: "hidden",
                    textShadow: "0px 0px 3px #022",
                    boxShadow: "0px 0px 5px -2px white",
                    cursor: "pointer"
                }
            };
        }
    }

	onMouseDown(ev) {
		ev.target.style.backgroundColor = "rgb(30, 144, 255)";
		this.draggingElem = ev.target;
	}

	onMouseUp(ev) {
		ev.target.style.backgroundColor = "rgb(83, 83, 83)";
		this.draggingElem = null;
	}

	onMouseLeave(ev) {
		ev.target.style.backgroundColor = "rgb(83, 83, 83)";
	}

	onMouseEnter(ev) {
		if (this.draggingElem && this.draggingElem === ev.target) {
			ev.target.style.backgroundColor = "rgb(30, 144, 255)";
		}
	}

    generator(value, key){
        const style = this.styles();
        return (
            <div style={style.list} key={"nodelist_" + value + "_" + key} value={value}
				onMouseDown={this.onMouseDown.bind(this)}
				onMouseUp={this.onMouseUp.bind(this)}
				onMouseLeave={this.onMouseLeave.bind(this)}
				onMouseEnter={this.onMouseEnter.bind(this)}
				onClick={this.onClick(value)}>{"・" + value}</div>
        );
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.block}>
                <div ref="select" style={style.select} size={this.state.nodeList.length}>
                    {this.state.nodeList.map(this.generator.bind(this))}
                </div>
            </div>
        );
    }
}
