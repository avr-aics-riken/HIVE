import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

export default class MenuNodeList extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            nodes: null,
            nodeList: this.props.store.getNodeNameList()
        };

        this.styles = this.styles.bind(this);
        this.props.store.on(Core.Constants.INITIALIZED, ()=>{
            this.setState({nodeList: this.props.store.getNodeNameList()});
        }.bind(this));
    }

    onDoubleClick(eve){
        var e = eve.currentTarget;
        this.props.action.addNodeByName(e.value);
    }

    styles() {
        return {
            block: {
                backgroundColor: "rgba(0, 0, 0, 0.25)",
                margin: "0px",
                padding: "0px",
                width: "100%",
                height: "300px",
                boxShadow: "0px 0px 1px 0px white inset",
                overflow: "auto"
            },
            select: {
                backgroundColor: "rgba(0, 0, 0, 0.25)",
                border: "none",
                color: "whitesmoke",
                fontSize: "large",
                margin: "0px",
                padding: "0px",
                width: "100%",
                minHeight: "100%",
                boxShadow: "0px 0px 1px 0px white inset",
                overflow: "auto"
            },
            list: {
                lineHeight: "30px",
                width: "100%",
                height: "30px",
                boxShadow: "0px 0px 1px 0px white inset"
            }
        }
    }

    generator(value, key){
        const style = this.styles();
        return (
            <option style={style.list} key={key} value={value} onDoubleClick={this.onDoubleClick.bind(this)}>{value}</option>
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
