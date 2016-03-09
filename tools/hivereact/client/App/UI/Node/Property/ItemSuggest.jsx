import React from "react";
import ReactDOM from "react-dom";
import Autosuggest from 'react-autosuggest';
import Core from '../../../Core';

/**
 * ノードプロパティアイテムサジェストリスト
 */
export default class ItemSuggest extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            value : this.props.initialParam.value,
            suggestions: this.getSuggestions('')
        };

        this.getSuggestions = this.getSuggestions.bind(this);
        this.getSuggestionValue = this.getSuggestionValue.bind(this);
        this.onSuggestionsUpdateRequested = this.onSuggestionsUpdateRequested.bind(this);
        this.renderSuggestion = this.renderSuggestion.bind(this);
        this.onChange = this.onChange.bind(this);
    }
    getSuggestions(value) {
        var pathList = this.props.store.getShaderList();
        var path = [];
        var flg = false;
        if(pathList){
            for(let i in pathList){
                path.push({name: pathList[i]});
            }
        }
        if(path.length === 0){return [];}
        const inputValue = value.trim().toLowerCase();
        const inputLength = inputValue.length;
        return inputLength === 0 ? [] : path.filter(nodename =>
             nodename.name.toLowerCase().indexOf(inputValue) !== -1
        );
    }
    onSuggestionsUpdateRequested({value}){
        this.setState({suggestions: this.getSuggestions(value)});
        if(value !== ''){
            this.props.changeFunc(this.props.initialParam.name, value);
        }
    }
    getSuggestionValue(suggestion){
        return suggestion.name;
    }
    renderSuggestion(suggestion){return (<span>{suggestion.name}</span>);}

    styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "2px 2px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "2px 2px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else{
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }
        };
        return {
            view : border.bind(this)(),
            key : {
                backgroundColor: "rgb(84,84,84)",
                color : "white",
                fontSize: "smaller",
                letterSpacing: "normal",
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
                paddingLeft : "4px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                border: "0px",
                outline: "0",
                borderRadius: "2px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "3px",
                marginTop: "1px",
                marginBottom: "1px",
                verticalAlign: "middle",
                padding: "1px",
                width : "152px",
                height: "19px",
                display: "inline-block",
            },
            suggestTheme: {
                // container:                   'react-autosuggest__container',
                // containerOpen:               'react-autosuggest__container--open',
                // input:                       'react-autosuggest__input',
                // suggestionsContainer:        'react-autosuggest__suggestions-container',
                // suggestion:                  'react-autosuggest__suggestion',
                // suggestionFocused:           'react-autosuggest__suggestion--focused',
                // sectionContainer:            'react-autosuggest__section-container',
                // sectionTitle:                'react-autosuggest__section-title',
                // sectionSuggestionsContainer: 'react-autosuggest__section-suggestions-container'
                container: {
                    display: "inline-block"
                },
                input: {
                    border: "0px",
                    borderRadius: "2px",
                    outline: "0",
                    color: "#333",
                    marginLeft: "3px",
                    marginTop: "1px",
                    marginBottom: "1px",
                    padding: "1px",
                    width: "153px",
                    height: "19px",
                    verticalAlign: "middle",
                    display: "inline-block"
                },
                suggestionsContainer: {
                    listStyle: "none",
                    margin: "0px 0px 0px 2px",
                    padding: "0px",
                    width: "154px",
                    position: "absolute"
                },
                suggestion: {
                    backgroundColor: "rgba(255, 255, 255, 0.8)",
                    borderLeft: "1px solid #333",
                    borderRight: "1px solid #333",
                    borderBottom: "1px solid #333",
                    color: "#333",
                    fontSize: "small",
                    margin: "0px",
                    padding: "2px 5px",
                    overflow: "hidden"
                },
                suggestionFocused: {
                    backgroundColor: "rgba(255, 255, 255, 1.0)",
                    color: "teal",
                    textShadow: "0px 0px 1px turquoise"
                }
            }
        };
    }

    onChange(eve, {newValue}){
        this.setState({value: newValue});
    }

    render () {
        const styles = this.styles.bind(this)();
        const {value, suggestions} = this.state;
        const inputProps = {
            placeholder: 'Type a Node name.',
            value,
            onChange: this.onChange
        };
        return (
            <div style={styles.view}>
                <div style={styles.key}>
                    {this.props.initialParam.name}
                </div>
                <Autosuggest theme={styles.suggestTheme}
                    suggestions={suggestions}
                    onSuggestionsUpdateRequested={this.onSuggestionsUpdateRequested}
                    getSuggestionValue={this.getSuggestionValue}
                    renderSuggestion={this.renderSuggestion}
                    inputProps={inputProps}
                    ref="suggest"
                />
            </div>
        );
    }
}
