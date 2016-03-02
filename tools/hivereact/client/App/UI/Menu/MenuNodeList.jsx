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
        this.props.store.on(Core.Constants.INITIALIZED, (()=>{
            this.setState({nodeList: this.props.store.getNodeNameList()});
        }).bind(this));
    }

    onDoubleClick(eve){
        var e = eve.currentTarget;
        this.props.action.addNodeByName(e.value);
    }

    styles() {
        return {
            block: {
                margin: "0px",
                padding: "0px",
                width: "100%",
                height: "250px",
                overflow: "auto"
            },
            select: {
                backgroundColor: "transparent",
                border: "none",
                color: "turquoise",
                fontSize: "large",
                margin: "0px",
                padding: "0px",
                width: "250px",
                minHeight: "100%",
                overflow: "auto"
            },
            list: {
                // backgroundColor: "rgba(128, 128, 128, 0.5)",
                backgroundColor: "#666",
                borderRadius: "5px",
                color: "turquoise",
                fontSize: "small",
                lineHeight: "24px",
                margin: "5px 3px",
                padding: "2px",
                width: "244px",
                height: "24px",
                overflow: "auto",
                textShadow: "0px 0px 3px #030",
                cursor: "pointer"
            }
        };
    }

    generator(value, key){
        const style = this.styles();
        return (
            <option style={style.list} key={key} value={value} onDoubleClick={this.onDoubleClick.bind(this)}>{"・" + value}</option>
        );
    }

    render(){
        const style = this.styles();
        return (
            <div style={style.block}>
                <select style={style.select} size={this.state.nodeList.length}>
                    {this.state.nodeList.map(this.generator.bind(this))}
                </select>
            </div>
        );
    }
}
