package daemon.provider;

public interface IProvider {

	public int getBusiness();

	public void execute(ProviderExecuteContext context);
}
